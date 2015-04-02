#include <xinu.h>

void strcpy(char*dest,char*src)
{
        while((*dest++ = *src++) != NULLCH);
}
