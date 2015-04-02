#include <xinu.h>

syscall pipconnect(	
			pipid32 pip,
			pid32 pid1,
			pid32 pid2
		)
{

	intmask mask;

	struct procent *prptr1;
	struct procent *prptr2;
	struct pipent  *pipptr;

	mask = disable();

	if(isbadpid(pid1) || isbadpid(pid2) || (pid1 == NULLPROC) || (pid2 == NULLPROC)){
		restore(mask);
		return (pid32) (SYSERR);

	}
	
	prptr1 = &proctab[pid1];
	prptr2 = &proctab[pid2];

	if(isbadpipid(pip)){
		restore(mask);
		return (pipid32) (SYSERR);
	}
	
	if((prptr1->prstate == PR_FREE) || (prptr2->prstate == PR_FREE)){
		restore(mask);
		return (pid32) (SYSERR);
	}

	pipptr = &piptab[pip];

	if(pipptr->pipstate != PIPE_USED){
		restore(mask);
		return (pipid32) (SYSERR);			
	}
	
	if(pipptr->pipOwner != getpid()){	
		restore(mask);
		return (pipid32) (SYSERR);
	}
	
	pipptr->pipstate = PIPE_CONNECTED;
	pipptr->pipWriter = pid1;
	pipptr->pipReader = pid2;		
	
	restore(mask);
	return OK;
}
