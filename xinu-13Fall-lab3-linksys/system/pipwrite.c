#include <xinu.h>

syscall pipwrite(
			pipid32 pip,
			char* buf,
			uint32 len 
		)
{

	intmask mask;				
	struct 	pipent	*pipptr;		
	int 	length;

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


	if(pipptr->pipWriter != getpid()) {	
		restore(mask);
		return (pipid32) (SYSERR);
	}
	
	int count = PIPE_SIZE - pipptr->bufCharCount;
	if(count < len && pipptr->bufCharCount != PIPE_SIZE){
		len = count;
	}

	for(length = 0 ; length < len ; length++) {

		wait(pipptr->fullSem);
		pipptr->buf[pipptr->tail] = buf[length];
		pipptr->tail = (pipptr->tail+1) % PIPE_SIZE;
		pipptr->bufCharCount++;
		signal(pipptr->emptySem);
	}

	restore(mask);				/* restore interrupts before normal return		*/
	return length;
}

