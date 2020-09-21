#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char c[80];

struct level7 {
    int number;
    char *buffer;
};

void m()
{
    time_t tloc;
    
    tloc = time((time_t *)0x0);
    printf("%s - %d\n", c, tloc);
    return;
}

int main(int argc, char **argv)
{
    struct level7 *structOne, *structTwo;
    FILE *__stream;
    
    structOne = malloc(sizeof(struct level7));
    structOne->number = 1;
    structOne->buffer = malloc(8);

    structTwo = malloc(sizeof(struct level7));
    structTwo->number = 1;
    structTwo->buffer = malloc(8);

    strcpy(structOne->buffer, argv[1]);
    strcpy(structTwo->buffer, argv[2]);

    __stream = fopen("/home/user/level8/.pass", "r");
    fgets(c, 4, __stream);
    puts("~~");
    return 0;
}