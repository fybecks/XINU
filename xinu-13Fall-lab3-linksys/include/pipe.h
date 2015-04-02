/* pipe.h */
/* Author: Fangyuan Hou */
/* October 9th, 2013*/

#ifndef NPIPE
#define	NPIPE		10
#endif

#define PIPE_SIZE 256

#define PIPE_FREE	0
#define PIPE_USED	1
#define PIPE_CONNECTED	2

#define isbadpipid(x)	((pipid32)(x) < 0 || (pipid32)(x) >= NPIPE)

struct pipent {		/* entry in the pipe table		*/
	int head;
	int tail;
	uint16	pipstate;	/* process state: PR_CURR, etc.		*/
	pid32   pipReader;
	pid32   pipWriter;
	pid32   pipOwner; 
	byte	buf[PIPE_SIZE]; /* pipe buffer				*/
	int bufCharCount;	
	sid32	fullSem;	/* semaphore on which process waits	*/
	sid32	emptySem;	/* id of the creating process		*/
	
	
};

extern struct pipent piptab[];
extern int32 pipcount;
