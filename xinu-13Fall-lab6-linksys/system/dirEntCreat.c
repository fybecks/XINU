#include<xinu.h>

/* Creates an entry for file or directory in the directory currently loaded  */

status createDirEntry(char *name, byte type, struct ldentry *dirEntry, bool8 isReplace)
{
	struct lflcblk * dirCblk = &lfltab[Nlfl+1];	
	struct lflcblk* parentDirCblk = &lfltab[Nlfl];	
	struct dentry devPtr;
	struct dentry parentDevPtr;

	devPtr.dvminor=Nlfl+1;	
	parentDevPtr.dvminor=Nlfl;	

	dirEntry->ld_size = 0;
	dirEntry->ld_ilist = LF_INULL;
	dirEntry->type = type;
	dirEntry->isUsed = (bool8)1;
	strcpy(dirEntry->ld_name,name);

	if(lflWrite(&devPtr,(char*)dirEntry,sizeof(struct ldentry)) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}

	/*Close the current directory*/
	if(lfflush(dirCblk) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}
	
	if(isReplace)
	{
		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return OK;
	}

	if(LF_FREE == parentDirCblk->lfstate)
	{
		dirCblk->lfstate = LF_FREE;

		wait(Lf_data.lf_mutex);
		Lf_data.lf_dir.lfd_size += sizeof(struct ldentry);
		Lf_data.lf_dirdirty = TRUE;
		signal(Lf_data.lf_mutex);
		return OK;
	}

	struct ldentry parentDirEntry;
	
	lflSeek(&parentDevPtr,parentDirCblk->lfpos - sizeof(struct ldentry));
	if(lflRead(&parentDevPtr,(char*)&parentDirEntry,sizeof(struct ldentry)) ==SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}
	
	parentDirEntry.ld_size += sizeof(struct ldentry);
	parentDirEntry.ld_ilist = dirCblk->firstIbId;

	lflSeek(&parentDevPtr,parentDirCblk->lfpos - sizeof(struct ldentry));

	if(lflWrite(&parentDevPtr,(char*)&parentDirEntry,sizeof(struct ldentry)) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}
	
	if(lfflush(parentDirCblk) == SYSERR)
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		
		return SYSERR;
	}
	dirCblk->lfstate = LF_FREE;
	parentDirCblk->lfstate = LF_FREE;
	return OK;
}
