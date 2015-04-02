/*  main_ec.c  - main */

#include <xinu.h>
#include <stdio.h>

pid32 globpid = 1;

void funcA(void){

	umsg32 msg = 0XAB; 

	kprintf("Before sending message %x \r\n", msg);

	sendb(globpid,msg);
	sendb(globpid,msg);
	
	kprintf("After sending message %x \r\n", msg);
	kprintf("Before receiving message %x \r\n", msg);

	receiveb();
	receiveb();
	
	kprintf("After receiving message %x \r\n", msg);
}

void funcB(pid32 p1){
	umsg32  msg = 0XCA;

	kprintf("Before sending message %x \r\n", msg);

	sendb(p1,msg);
	sendb(p1,msg);

	kprintf("After sending message %x \r\n", msg);	
	kprintf("Before receiving message %x \r\n", msg);

	receiveb();
	receiveb();

	kprintf("After receiving message %x \r\n", msg);
}

int main(int argc, char **argv)
{
	
	pid32 proc1, proc2;
	proc1 = create(funcA, 1024, 20, "process 1", 0);
	// kprintf("After sending message \r\n");
 	proc2 = create(funcB, 1024, 20, "process 2", 1, proc1);
	globpid = proc2;

	resume(proc1);
	resume(proc2);
	
	return OK;

}
