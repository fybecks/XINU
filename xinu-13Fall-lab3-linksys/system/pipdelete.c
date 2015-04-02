#include <xinu.h>

syscall pipdelete(	
		pipid32 pip  
	)
{
	struct pipent *pipptr;
	intmask mask;

	mask = disable();
	
	if(isbadpipid(pip)){
		restore(mask);
		return (pipid32) (SYSERR);
	}

	pipptr = &piptab[pip];

	if(pipptr->pipstate == PIPE_FREE){
		restore(mask);
		return (pipid32) (SYSERR);

	}
	
	if(pipptr->pipOwner != getpid()){

		restore(mask);
		return (pipid32) (SYSERR);
		
	}

	pipptr->pipstate = PIPE_FREE;
	memset(pipptr->buf, '\0', PIPE_SIZE);	
	
	pipptr->pipOwner = -1;
	pipptr->pipReader = -1;
	pipptr->pipWriter = -1;
	pipptr->bufCharCount = -1;
	pipcount--;

	semdelete(pipptr->fullSem);
	semdelete(pipptr->emptySem);
	restore(mask);
	return OK;

}
