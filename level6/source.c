#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct buffer {
    char *buff;
};

struct code {
    int (*code)();
};

void n(void)
{
    system("/bin/cat /home/user/level7/.pass");
    return;
}

void m(void *argv, int argc, char *param_3, int param_4, int param_5)
{
    puts("Nope");
    return;
}

void main(char **argv, int argc)
{
    struct buffer *dest;
    struct code *ppcVar1;
    
    dest = malloc(sizeof(struct buffer));
    ppcVar1 = malloc(sizeof(struct code));
    ppcVar1->code = m;
    strcpy(dest->buff, argv[1]);
    ppcVar1->code();
    return;
}