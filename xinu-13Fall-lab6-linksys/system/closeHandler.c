#include <xinu.h>

status closeHandler(char *fileName,struct lflcblk* lfptr)
{
	struct lflcblk * dirCblk = &lfltab[Nlfl+1];	/*last entry is used for modifying the directory in which file is getting created.*/
	struct lflcblk* parentDirCblk = &lfltab[Nlfl];	/*second last entry is used for parent of the directory in which file is getting created*/
	struct dentry devPtr;
	struct dentry parentDevPtr;
	struct ldentry tempEntry;
	struct ldentry*dirEntry = &tempEntry;
	devPtr.dvminor=Nlfl+1;	
	parentDevPtr.dvminor=Nlfl;	
	bool8 found = 0;
	while(lflRead(&devPtr,(char*)dirEntry,sizeof(struct ldentry)) == sizeof(struct ldentry))
	{
		if(strcmp(dirEntry->ld_name,fileName) && dirEntry->isUsed)
		{
			found = 1;
			break;
		}
	}
	if(!found)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}
	
	dirEntry->ld_ilist = lfptr->firstIbId;
	dirEntry->ld_size = lfptr->fileSize;
	
/*	uint32 writePos = dirCblk->lfpos - sizeof(struct ldentry); */

/*	if(lflSeek(&devPtr,writePos) == SYSERR)
	{

	} */

	if(lflWrite(&devPtr,(char*)dirEntry,sizeof(struct ldentry)) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}

	/*Close the directory*/
	if(lfflush(dirCblk) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		
		return SYSERR;
	}
	dirCblk->lfstate = LF_FREE;
	parentDirCblk->lfstate = LF_FREE;
	
	return OK;

}

