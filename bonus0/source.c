#include <string.h>
#include <stdio.h>
#include <unistd.h>

void p(char *param_1, char *dash)
{
    char *str;
    char bigBuffer[4104];
    
    puts(dash);
    read(stdin, bigBuffer, 4096);
    str = strchr(bigBuffer, 10); //10 is ASCII for '\n'
    *str = '\0';
    strncpy(param_1, bigBuffer, 20);
    return;
}

void pp(char *buffer)
{
    char smallBufferOne[20];
    char smallBufferTwo[20];
    int len;
 
    p(smallBufferOne," - ");
    p(smallBufferTwo," - ");
    strcpy(buffer, smallBufferOne);

    len = strlen(buffer);
    buffer[len] = ' ';
    strcat(buffer,smallBufferTwo);
    return;
}


int main(void)
{
    char buffer[54];
    
    pp(buffer);
    puts(buffer);
    return 0;
}