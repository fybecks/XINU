#include<xinu.h>

 /* This function initializes tokens array to name of files/directories. */

int tokenize(char *path,char tokens[][LF_NAME_LEN])
{
	int i = 0;
	int tokenCount = 0;

	if(path[i] == '/' && path[i+1] == '\0')
	{
		tokens[0][0] = '/';
		tokens[0][1] = '\0';
		return 1;
	}
	int index = 0;
	
	while(tokenCount < LF_PATH_DEPTH)
	{
		if(path[index] == '/')
		{
			++index;
		}
		for(i=0; path[index] && path[index] != '/' && i < LF_NAME_LEN-1; ++i)
		{
			tokens[tokenCount][i]=path[index];	
			++index;
		}
		if(i)
		{
			if(i >= LF_NAME_LEN-1)
			{

				return SYSERR;
			}
			tokens[tokenCount][i] = '\0';

			++tokenCount;
			
		}
		if(path[index] == NULLCH || (path[index] == '/' && path[index+1] == NULLCH))
		{
			break;
		}
	}
	return tokenCount;
}
