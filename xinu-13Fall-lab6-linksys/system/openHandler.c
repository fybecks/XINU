#include <xinu.h>

status openHandler(char *fileName,struct ldentry *dirEntry,int32 mbits)
{
	struct lflcblk * dirCblk = &lfltab[Nlfl+1];	/*last entry is used for modifying the directory in which file is getting created.*/
	struct lflcblk* parentDirCblk = &lfltab[Nlfl];	/*second last entry is used for parent of the directory in which file is getting created*/
	struct dentry devPtr;
	struct dentry parentDevPtr;
	devPtr.dvminor=Nlfl+1;	
	parentDevPtr.dvminor=Nlfl;	
	uint32 replacePos = 0;
	bool8 isRPosInitialized = 0;

	/*
	 * Keep on reding the entries in the parent directory unless you find a match
	 */
	while(lflRead(&devPtr,(char*)dirEntry,sizeof(struct ldentry)) == sizeof(struct ldentry))
	{

		/*
		 * If we find a deleted entry we can reuse it 
		 * in case we create a new file.
		 */
		if(!dirEntry->isUsed)
		{
			if(!isRPosInitialized)
			{
				replacePos = dirCblk->lfpos - sizeof(struct ldentry);
				isRPosInitialized = 1;
			}
			continue;
		}
		/*
		 * We found a match.
		 */
		if(strcmp(dirEntry->ld_name,fileName) && dirEntry->isUsed)
		{

			if( LF_TYPE_DIR == dirEntry->type)
			{	
				/*Trying to open a directory	*/
				dirCblk->lfstate = LF_FREE;
				parentDirCblk->lfstate = LF_FREE;
				return SYSERR;
			}
			if (mbits & LF_MODE_N) 
			{	/* file must not exist	*/
				dirCblk->lfstate = LF_FREE;
				parentDirCblk->lfstate = LF_FREE;
				return SYSERR;

			}
			dirCblk->lfstate = LF_FREE;
			parentDirCblk->lfstate = LF_FREE;
			return OK;
		}
	}
	//File Not Found
	/*
	 * If we are opening a file and file doesn't exist
	 * then return error.
	 */
	if(mbits & LF_MODE_O)
	{
		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;

		return SYSERR;
	}


	/*
	 * Create the file
	 */
	if(SYSERR == createDirEntry(fileName,LF_TYPE_FILE,dirEntry,isRPosInitialized))
	{

		dirCblk->lfstate = LF_FREE;
		parentDirCblk->lfstate = LF_FREE;
		return SYSERR;
	}
	return OK;
}
