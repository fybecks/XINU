#include<xinu.h>

/* This funtion is to make a new directory. */

status mkDir(char *path)
{
	char pathTokens[LF_PATH_DEPTH][LF_NAME_LEN];
 
	int pathDepth = tokenize(path, pathTokens);

	if(pathDepth == SYSERR)
		return SYSERR;

	if(pathDepth == 1 && pathTokens[0][0] == PATH_SEPARATOR)
		return SYSERR;

	wait(lfDirCblkMutex);
 
	if(moveToDir(pathTokens, pathDepth-1) == SYSERR)
	{
		signal(lfDirCblkMutex);
		return SYSERR;
	}	

	struct lflcblk * dirCblk = &lfltab[Nlfl + 1];	/*last entry is used for modifying the directory in which file is getting created.*/
	struct lflcblk * parentDirCblk = &lfltab[Nlfl];	
	struct dentry devPtr;
	struct dentry parentDevPtr;
	struct ldentry tempDirEntry;
	struct ldentry *dirEntry = &tempDirEntry;
	char * dirName = pathTokens[pathDepth - 1];
	uint32 replacePos = 0;
	bool8 isRPosInitialized = 0;
	devPtr.dvminor = Nlfl+1;	
	parentDevPtr.dvminor = Nlfl;	

	/* find out whether there is any free entry in parent directory. */
	while(lflRead(&devPtr,(char*)dirEntry,sizeof(struct ldentry)) == sizeof(struct ldentry))
	{

		if(!dirEntry->isUsed)
		{
			if(!isRPosInitialized)
			{
				replacePos = dirCblk->lfpos - sizeof(struct ldentry);
				isRPosInitialized = 1;
			}
			continue;
		}
		if(strcmp(dirEntry->ld_name, dirName) && dirEntry->isUsed)
		{

			dirCblk->lfstate = LF_FREE;
			parentDirCblk->lfstate = LF_FREE;
			signal(lfDirCblkMutex);
			return SYSERR;
		}
	}

	 /* Create a new dir entry */
	if(SYSERR == createDirEntry(dirName, LF_TYPE_DIR, dirEntry, isRPosInitialized))
	{
		signal(lfDirCblkMutex);
		return SYSERR;
	}

	signal(lfDirCblkMutex);
	return OK;
}
