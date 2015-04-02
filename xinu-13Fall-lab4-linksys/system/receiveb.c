#include<xinu.h>

umsg32 receiveb(void)
{
	intmask mask;
	struct procent *prptr;
	umsg32 msg;
	struct procent *prptr1;
	pid32 sendpid;

	mask = disable();
	
	prptr = &proctab[currpid];

	if(prptr->prhasmsg == FALSE){
		
		prptr->prstate = PR_RECV;
		resched();
	}
	
	if(nonempty(prptr->sendqueue)){
		msg = prptr->prmsg;
	
		while((sendpid = dequeue(prptr->sendqueue)) != EMPTY){
			prptr1 = &proctab[sendpid];
			if(prptr1->prstate == PR_SND){
				prptr->prmsg = prptr1->sndmsg;
				prptr1->sndflag = FALSE;

				ready(sendpid, RESCHED_NO);
				break;

			}
			else{
				continue;
			}

		}

	}
	else {

	msg = prptr->prmsg;
	prptr->prhasmsg = FALSE;

	}
	restore(mask);
	return msg;
}
