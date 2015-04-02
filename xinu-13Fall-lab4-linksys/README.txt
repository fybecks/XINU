In this lab4, I have completed both "Blocked Message Passing" and extra credit.

Newly created system calls:

sendb(): I used sendqueue field of receiving process to put the message in a blocking queue by enqueue fucntion, if the receiving process' msg field is already filled.  

receiveb(): If the sendqueue field of the current process is not empty, we are going to unblock the process from the sendqueue by dequeue function and return the message. 

sendq(): It had the same idea as sendb, except that I used an array to implementthe message queue.

receiveq(): It had almost the same idea with receiveb, except that it also used the array to dequeue the message.

modifed files:
process.h: added PR_SND state, and five fields: sndmsg, sndflag, sendqueue, prmsgq[],and msgqsize in struct procent. 
kill.c: added code to handle the kill of process with PR_SND state.
create.c: initialized sndflag, sendqueue, and msgqsize.
Makerules: to compile the modified the added code.
main_ec.c: to implement deadlock.  
