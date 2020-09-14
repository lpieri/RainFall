#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int m;

void o(void)
{
    system("/bin/sh");
                        /* WARNING: Subroutine does not return */
    _exit(1);
}

void n(void)
{
    char buffer[520];
    
    fgets(buffer,4,stdin);
    printf(buffer);
                        /* WARNING: Subroutine does not return */
    exit(1);
}

void main(void)
{
    n();
    return;
}