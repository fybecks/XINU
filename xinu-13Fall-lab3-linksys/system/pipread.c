#include <xinu.h>

syscall pipread(
	 pipid32		pip,		
	 char*			buf,		
	 uint32			len		
	)
{	
	intmask mask;				
	struct 	pipent	*pipptr;		

	mask = disable();			

	if(isbadpipid(pip)) {
		restore(mask);			
		return (pipid32) (SYSERR);
	}

	if(buf == NULL){
		restore(mask);			
		return (pipid32) (SYSERR);
	}

	pipptr = &piptab[pip];
	if(pipptr->pipstate != PIPE_CONNECTED){	
		restore(mask);			
		return (pipid32) (SYSERR);
	}

	if(pipptr->pipOwner == PR_FREE || pipptr->pipReader == PR_FREE || pipptr->pipWriter == PR_FREE) {
		restore(mask);
		return (pipid32) (SYSERR);
	}

	if(pipptr->pipReader != getpid()) {	
		restore(mask);
		return (pipid32) (SYSERR);
	}

	int length;
	if(pipptr->bufCharCount < len && pipptr->bufCharCount > 0){
		len = pipptr->bufCharCount;
	}

	for(length = 0 ; length < len ; length++){
		wait(pipptr->emptySem);
		buf[length] = pipptr->buf[pipptr->head];
		pipptr->head = (pipptr->head+1) % PIPE_SIZE; 
		pipptr->bufCharCount--;
		signal(pipptr->fullSem);
	}

	restore(mask);				
	return length;		
}
