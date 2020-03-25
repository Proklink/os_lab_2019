#include "revert_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RevertString(char *str)
{
    int i=0;
    char temp;
    int len = strlen(str)-1;
	while( i < len/2 )
    {   
        temp = str[i];
        str[i] = str[len-i];
        str[len-i] = temp;
        i++;
    }
}

