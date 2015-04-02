#include<xinu.h>

bool8 isEqualFilePath(char one[][LF_NAME_LEN], int len1,char two[][LF_NAME_LEN], int len2)
{
        int i = 0;
        if(len1 != len2)
                return 0;

        for(i=0; i < len1; ++i)
        {
                if(!strcmp(one[i], two[i]))
                {
                        return 0;
                }
        }
        return 1;
}
