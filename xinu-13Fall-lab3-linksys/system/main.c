/* main.c - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void consume(pipid32 pip);
void produce(pipid32 pip);

/************************************************************************/
/* */
/* main - main program for testing Xinu */
/* */
/************************************************************************/

int main(int argc, char **argv)
{
  	pipid32 pip;
  	pid32	reader;
  	pid32	writer;
  
  	if(SYSERR == (pip = pipcreate())) {
    		return 0;
  	}
    
  	if(SYSERR == (reader = create(consume, 8196, 20, "read", 1, pip))) {
    		return 0;
  	}
  
  	if(SYSERR == (writer = create(produce, 8196, 20, "write", 1, pip))) {
    		return 0;
  	}

  	if((pipconnect(pip, writer, reader) == SYSERR)) {
    		return 0;
  	}
  	
  	resume(writer);
  	resume(reader);

	while(1){
		char command = getchar();
		if(command == 10){
			resume(reader);
		}
		if(command == 'q'){
			break;
		}
		
	}
  	  
  	kill(reader);
  	kill(writer);
  
  	pipdisconnect(pip);
  	pipdelete(pip);
   
  	return OK;
}

void consume(pipid32 pip) {
  	char msg[4];
  	int read = 0;
  	int i;
  	int len = 4;
  
  while(1) {
     	
		for(i = 0; i < 6; i++){
			read = pipread(pip, msg, len);
			if(read == 0) 
      				continue;    			   			
		//kprintf("read:%d\n\r",read);
		int j;
    			for(j=0; j < read;j++){
				kprintf("%c",msg[j]);
	//			kprintf("\n\r");
			}
			kprintf("\n\r");
		}	
    		suspend(getpid());
  	}


}

void produce(pipid32 pip) {
  
char input[15] = "1234567890123\0"; 

	int write = 0;

    while(1) {
  	write = pipwrite(pip, input, 10);
    }
}
