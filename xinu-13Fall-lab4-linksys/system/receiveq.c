#include<xinu.h>

umsg32 receiveq(void)
{
	intmask mask;
	struct procent *prptr;
	struct procent *prptr1;
	umsg32	msg;
	uint32 i;
	pid32 sendpid;
	
	mask = disable();

	prptr = &proctab[currpid];

	if(prptr->prhasmsg == FALSE){
		prptr->prstate = PR_RECV;
		resched();
	} 

	if(prptr->msgqsize == 0){
			if(nonempty(prptr->sendqueue)){

	
		while((sendpid = dequeue(prptr->sendqueue)) != EMPTY){
			prptr1 = &proctab[sendpid];
			if(prptr1->prstate == PR_SND){
					prptr->prmsgq[prptr->msgqsize] = prptr1->sndmsg;
				prptr->msgqsize++;
					
				prptr1->sndflag = FALSE;

				ready(sendpid, RESCHED_NO);
				break;

			}
			else	{
					continue;
				}

			}


			msg = prptr->prmsgq[0];
			for(i = prptr->msgqsize - 1; i > 0;i--){
				prptr->prmsgq[i - 1] = prptr->prmsgq[i];
			}
			prptr->msgqsize--;
		
		}

		 else {
				prptr->prhasmsg = FALSE;
				prptr->prstate = PR_RECV;
				resched();
			}

	} else{
			msg = prptr->prmsgq[0];
			for(i = prptr->msgqsize - 1; i > 0;i--){
				prptr->prmsgq[i - 1] = prptr->prmsgq[i];
			}
			prptr->msgqsize--;

			if(prptr->msgqsize == 0 && isempty(prptr->sendqueue) ){
				prptr->prhasmsg = FALSE;
			}

	}

	restore(mask);
	return msg;
}
