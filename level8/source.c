#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct authentication {
    char buffer[32];
    int number;
};

struct authentication *auth;
char *service;

int main(void)
{
    char line[125];

    do {
        printf("%p, %p \n", auth, service);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        if (strncmp(line, "auth", 5) == 0) {
            auth = malloc(sizeof(auth));
            memset(auth, 0, sizeof(auth));
            if (strlen(line + 5) < 31) {
                strcpy(auth->buffer, line + 5);
            }
        }

        if (strncmp(line, "reset", 5) == 0) {
            free(auth);
        }

        if (strncmp(line, "service", 6) == 0) {
            service = strdup(line + 7);
        }

        if (strncmp(line, "login", 5) == 0) {
            if (auth->number) {
                system("/bin/sh");
            } else {
                fwrite("Password:\n",1,10,stdout);
            }
        }
    } while( true );
}