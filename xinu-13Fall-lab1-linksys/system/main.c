/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/************************************************************************/
/*									*/
/* main - main program for testing Xinu					*/
/*									*/
/************************************************************************/

int main(int argc, char **argv)
{
	uint32 i;

	kprintf("Hello World!\n");
	for(i = 0; i < NPROC; i++){
		print_proc_info(i);	
	}
		kprintf("Before creating 2 processes.\n");
		resume(create(sleep,1024,20,"process 1",1,10));	
		resume(create(sleep,1024,20,"process 2",1,10));
		
	
	for(i = 0; i < NPROC; i++){
		print_proc_info(i);	
	}		
		
	return OK;
}
