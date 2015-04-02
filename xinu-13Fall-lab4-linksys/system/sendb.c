#include<xinu.h>

syscall sendb(
		pid32      pid,
		umsg32	   msg
		)
{
	intmask mask;
	struct procent *prptr;
	struct procent *prptr1;

	mask = disable();

	if(isbadpid(pid)){
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	if(prptr->prstate == PR_FREE){
		restore(mask);
		return SYSERR;
	}

	prptr1 = &proctab[currpid];

	if(prptr->prhasmsg){
		prptr1->sndmsg = msg;
		prptr1->sndflag = TRUE;
		prptr1->prstate = PR_SND;
		
		enqueue(currpid, prptr->sendqueue);
		resched();		
	} else {
	
		
		prptr->prmsg = msg;
		prptr->prhasmsg = TRUE;

	}
	
	if(prptr->prstate == PR_RECV){
		ready(pid, RESCHED_YES);
	} else if(prptr->prstate == PR_RECTIM){
		unsleep(pid);
		ready(pid, RESCHED_YES);
	}	

	restore(mask);
	return OK;

}
