/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/************************************************************************/
/*									*/
/* main - main program for testing Xinu					*/
/*									*/
/************************************************************************/

void consume(pipid32 pip);
void produce(pipid32 pip);

int main(int argc, char **argv)
{
  
  pipid32	pip;
  pid32	reader;
  pid32	writer;
  char c;
  if(SYSERR == (pip = pipcreate())) {
    kprintf("Error: Creating pipe\n\r");
    return 0;
  }

  if(SYSERR == (reader = create(consume, 4098, 25, "read", 1, pip))) {
    kprintf("Error: Creating consumer process\n\r");
    return 0;
  }

  if(SYSERR == (writer = create(produce, 4098, 25, "write", 1, pip))) {
    kprintf("Error: Creating producer process\n\r");
    return 0;
  }

  if((pipconnect(pip, writer, reader) == SYSERR)) {
    kprintf("Error: Connecting Pipe\n\r");
    return 0;
  }

  resume(writer);
  resume(reader);


  while(1)
	{
		c = getchar();

		if(c == 'q')
			break;

		resume(reader);
	}


  pipdisconnect(pip);
  pipdelete(pip);
  kill(reader);
  kill(writer);

	return OK;
}


void consume(pipid32 pip) {
  int	len = 3;
  char msg[3];
  int read = 0;
  int i = 0, j = 0;
  while(1) {

    for(i = 0; i < 5; i++)
		{
      if((read = pipread(pip, msg, len)) == SYSERR) {
        kprintf("Error: Reading\n\r");
        return;
      }
      kprintf("Message read: ");
      for(j = 0; j <read; j++)
        kprintf("%c", msg[j]);
      kprintf("\n\r");
      //kprintf("Length read: %d\n\r", read);
    }
		suspend(getpid());
  }
}

void produce(pipid32 pip) {
  
  char msg[6] = "abcde";
  int	write = 0;

	while(1)
	{
			if((write = pipwrite(pip, msg, 4)) == SYSERR)
			{
				kprintf("Error: Writing\n\r");
        return;
			}
      //kprintf("Length written: %d\n\r", write);
		
	}
}


