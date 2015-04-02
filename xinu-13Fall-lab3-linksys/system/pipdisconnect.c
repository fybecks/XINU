#include <xinu.h>

syscall pipdisconnect(			
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

	if(pipptr->pipstate != PIPE_CONNECTED){
		restore(mask);
		return (pipid32) (SYSERR);
	}

	if(pipptr->pipOwner != getpid()){
		restore(mask);
		return (pipid32) (SYSERR);
	}

	pipptr->pipstate = PIPE_USED;
	pipptr->pipReader = 0;
	pipptr->pipWriter = 0;

	semdelete(pipptr->fullSem);
	semdelete(pipptr->emptySem);

	restore(mask);
	return OK;
}
