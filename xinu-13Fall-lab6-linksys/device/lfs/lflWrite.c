/* lflWrite.c  -  lfWrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lflWrite  --  write data to a previously opened local disk file
 *------------------------------------------------------------------------
 */
devcall	lflWrite (
	  struct dentry *devptr,	/* entry in device switch table */
	  char	*buff,			/* buffer holding data to write	*/
	  int32	count			/* number of bytes to write	*/
	)
{
	int32		i;		/* number of bytes written	*/

	if (count < 0) {
		return SYSERR;
	}
	struct	lflcblk	*lfptr;		/* ptr to open file table entry */

	lfptr = &lfltab[devptr->dvminor];
	wait(lfptr->lfmutex);

	if (lfptr->lfstate != LF_USED) {
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	if (lfptr->lfpos > lfptr->fileSize) {
		signal(lfptr->lfmutex);
		return SYSERR;
	}

	i = 0 ;

	do
	{
		if (lfptr->lfbyte >= &lfptr->lfdblock[LF_BLKSIZ]) 
			lfsetup(lfptr);

		if (lfptr->lfpos >= lfptr->fileSize ) {
			lfptr->fileSize++;
		}

		*lfptr->lfbyte++ = *buff++;
		lfptr->lfpos++;
		lfptr->lfdbdirty = TRUE;

	} while(++i < count);
	
	signal(lfptr->lfmutex);

	return count;
}
