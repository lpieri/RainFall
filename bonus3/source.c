#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *filePtr;
    char bufferOne[66];
    char bufferTwo[65];
    
    filePtr = fopen("/home/user/end/.pass", "r");
    if ((filePtr == NULL) || (argc != 2))
        return -1;

    fread(bufferOne, 1, 66, filePtr);
    bufferOne[atoi(argv[1])] = '\0';

    fread(bufferTwo, 1, 65, filePtr);
    fclose(filePtr);

    if (strcmp(bufferOne, argv[1]) == 0)
        execl("/bin/sh", "sh", 0);
    else
        puts(bufferTwo);

    return 0;
}