/* lfsOpen.c  -  lfsOpen */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lfsOpen - open a file and allocate a local file pseudo-device
 *------------------------------------------------------------------------
 */
devcall	lfsOpen (
	 struct	dentry	*devptr,	/* entry in device switch table	*/
	 char	*path,			/* complete path of file to open		*/
	 char	*mode			/* mode chars: 'r' 'w' 'o' 'n'	*/
	)
{
	did32		lfnext;		/* minor number of an unused	*/
					/*    file pseudo-device	*/
	struct	lflcblk	*lfptr;		/* ptr to open file table entry	*/
	int32	mbits;			/* mode bits			*/


	char pathTokens[LF_PATH_DEPTH][LF_NAME_LEN];  
	int pathDepth = tokenize(path,pathTokens);
	if(pathDepth == SYSERR)
	{

		return SYSERR;
	}
	if(1 == pathDepth && PATH_SEPARATOR==pathTokens[0][0])
	{

		return SYSERR;
	}
	/* Parse mode argument and convert to binary */

	mbits = lfgetmode(mode);
	if (mbits == SYSERR) {
		return SYSERR;
	}

	/* If named file is already open, return SYSERR */

	lfnext = SYSERR;
	wait(lfDirCblkMutex);
	 if(isFileOpen(pathTokens,pathDepth,&lfnext))
	{
		signal(lfDirCblkMutex);
		return SYSERR;
	} 

	if (lfnext == SYSERR) {	/* no slave file devices are available	*/

		signal(lfDirCblkMutex);
		return SYSERR;
	}

	struct ldentry fileInfo;

	if(moveToDir(pathTokens,pathDepth-1) == SYSERR)
	{

		signal(lfDirCblkMutex);
		return SYSERR;
	}
	
	if(openHandler(pathTokens[pathDepth-1],&fileInfo,mbits) == SYSERR)
	{

		signal(lfDirCblkMutex);
		return SYSERR;
	}   
	

	/* Initialize the local file pseudo-device */
	lfptr = &lfltab[lfnext];
	lfptr->lfstate = LF_USED;
	lfptr->lfmode = mbits & LF_MODE_RW;

	/* File starts at position 0 */

	lfptr->lfpos     = 0;

	/* Neither index block nor data block are initially valid	*/

	lfptr->lfinum    = LF_INULL;
	lfptr->lfdnum    = LF_DNULL;

	/* Initialize byte pointer to address beyond the end of the	*/
	/*	buffer (i.e., invalid pointer triggers setup)		*/

	lfptr->lfbyte = &lfptr->lfdblock[LF_BLKSIZ];
	lfptr->lfibdirty = FALSE;
	lfptr->lfdbdirty = FALSE;

	lfptr->fileSize = fileInfo.ld_size;
	lfptr->firstIbId = fileInfo.ld_ilist;
	memcpy(lfptr->path,pathTokens,LF_NAME_LEN * LF_PATH_DEPTH);
	lfptr->depth = pathDepth;

	signal(lfDirCblkMutex);
	return lfptr->lfdev;
}
