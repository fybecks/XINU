/* lflRead.c  -  lfRead */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lflRead  --  read from a previously opened local file
 *------------------------------------------------------------------------
 */
devcall	lflRead (
	  struct dentry *devptr,	/* entry in device switch table */
	  char	*buff,			/* buffer to hold bytes		*/
	  int32	count			/* max bytes to read		*/
	)
{
	uint32	numread;		/* number of bytes read		*/
	// 	int32   nxtbyte;                /* character or SYSERR/EOF      */	

	if (count < 0) {
		return SYSERR;
	}

	struct	lflcblk	*lfptr;		
	lfptr = &lfltab[devptr->dvminor];
	wait(lfptr->lfmutex);

	if (lfptr->lfstate != LF_USED) {
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	if (lfptr->lfpos >= lfptr->fileSize) {
		signal(lfptr->lfmutex);
		return EOF;
	}

	numread = 0;
	do
	{
		/* If byte pointer is beyond the current data block, */
		/*	set up a new data block			     */
		if (lfptr->lfbyte >= &lfptr->lfdblock[LF_BLKSIZ]) 
		{
			lfsetup(lfptr);
		}
		*buff++ = (char)(0XFF & *lfptr->lfbyte++);

		lfptr->lfpos++;
	}while(++numread < count && lfptr->lfpos < lfptr->fileSize);

	signal(lfptr->lfmutex);

	return (numread == 0 ?EOF:numread);
}
