#include <xinu.h>

bool8 strcmp(char* one, char* two)
{
        while(*one != NULLCH && *one == *two)
        {
                one++;
                two++;
        }
        return (*one == *two) &&(*one == NULLCH);
}
