/* print_proc_info.c - print_proc_info */

#include <xinu.h>
#include <string.h>

/*------------------------------------------------------------------------
*  print_proc_info  -  Prints the info in proctab for a given pid
*------------------------------------------------------------------------
*/
syscall print_proc_info(
       pid32   pid   /* process ID     */
     )

{
     intmask mask;     /* saved interrupt mask   */
     struct procent *prptr;

     /*-----  Declare variables to hold values  ------*/
     /*-----  ------------------------------- --------*/

     mask = disable();
     prptr = &proctab[pid];

     /*-----  Your code for fetching process specific info and displaying them goes here -----*/
     /*-----  ------------------------------- --------*/
     /*-----  ------------------------------- --------*/
     	if(pid >= NPROC || isbadpid(pid)){
		return (SYSERR);
	}
	else{
		kprintf("%10s\t %s\t %s\t %s\r\n","Proc Name", "Proc Prio","Proc State","Proc Stk Ptr");
		kprintf("%10s\t %d\t %u\t %x\r\n", prptr->prname, prptr->prprio, prptr->prstate, &prptr->prstkptr);
		
     	restore(mask);
	return OK;
	}

}
