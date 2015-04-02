#include<xinu.h>

/* lfsControl.c - lfsControl */


/*------------------------------------------------------------------------
 * lfsControl - Provide control functions for a local file system 
 *------------------------------------------------------------------------
 */
devcall	lfsControl (
	 struct dentry	*devptr,	
	 int32	func,			/* a control function		*/
	 int32	arg1,			/* argument #1			*/
	 int32	arg2			/* argument #2			*/
	)
{
	int retval;
	switch(func)
	{
		case LF_CTL_MKDIR:
			{
				retval = mkDir((char*)arg1);
				return retval;
			}
		default:
			return SYSERR;
	}
	return OK;	

}
