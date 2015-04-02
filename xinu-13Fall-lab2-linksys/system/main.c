/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/************************************************************************/
/*									*/
/* main - main program for testing Xinu					*/
/*									*/
/************************************************************************/

void funcA(void){
	while(1){
		kprintf("This is function A\n");
	}
}

void funcB(void){
	while(1){
		kprintf("This is function B\n");
	}
}

int main(int argc, char **argv)
{
	kprintf("Hello World!\n");	
	resume(create(&funcA,1024,20,"process A",0));
	resume(create(&funcB,1024,15,"process B",0));	

	return OK;
}



