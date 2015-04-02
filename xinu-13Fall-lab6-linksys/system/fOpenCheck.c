#include<xinu.h>

bool8 isFileOpen(char pathTokens[][LF_NAME_LEN], int pathDepth, int *firstFreeSlot)
{
	struct lflcblk * lfptr;
	int i;
	for (i = 0; i < Nlfl; i++)
	{	
		lfptr = &lfltab[i];

		if (lfptr->lfstate == LF_FREE){
			if(SYSERR == *firstFreeSlot)
				*firstFreeSlot = i;
		}
		else{
			if(isEqualFilePath(lfptr->path, lfptr->depth, pathTokens, pathDepth))
			{

				signal(lfDirCblkMutex);
				return 1;
			}
		}
	}
	return 0;
}
