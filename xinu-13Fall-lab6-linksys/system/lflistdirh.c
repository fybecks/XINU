#include <xinu.h>

int lflistdirh(did32 diskdev, char *directorypath){

	char pathTokens[LF_PATH_DEPTH][LF_NAME_LEN];
  
       	int depth = tokenize(directorypath, pathTokens);

	wait(lfDirCblkMutex);

	if(pathTokens[0][0] == '/' && pathTokens[0][1] == '\0')
		depth = 0;

	int dirDep = moveToDir(pathTokens, depth);

	if(dirDep == SYSERR){
               signal(lfDirCblkMutex);
               return SYSERR;
       }

       struct dentry devEnt = devtab[diskdev];
       struct ldentry ldEnt;

       devEnt.dvminor = Nlfl+1;        

	while(lflRead(&devEnt, (char*) &ldEnt, sizeof(struct ldentry)) == sizeof(struct ldentry)){
		if(depth != 0)
			kprintf("%s/%s\r\n",directorypath , ldEnt.ld_name);
		else
			 kprintf("/%s\r\n", ldEnt.ld_name);
	}		
		
	signal(lfDirCblkMutex);
	return OK;
}
