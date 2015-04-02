#include <xinu.h>

static pipid32 newpip(void);

syscall pipcreate(void)
{

	intmask mask;				/* interrupt mask				*/
	pipid32 pip;				/* pipe id					*/

	mask = disable();			/* disable interrupts at start of function	*/

	pip = newpip();
	pipcount++;					
	restore(mask);				/* restore interrupts before normal return	*/
	return pip;
}

local pipid32 newpip(void)
{
	int32 	i;				
	static	pipid32 nextpip = 1;		
	for(i = 0 ; i < NPIPE ; i++) {
		nextpip %= NPIPE;		
		if (piptab[nextpip].pipstate == PIPE_FREE) {
			piptab[nextpip].pipstate = PIPE_USED;
			piptab[nextpip].pipOwner = getpid();
			return nextpip++;
		} else {
			nextpip++;
		}
	}
	return (pipid32)SYSERR;
}
