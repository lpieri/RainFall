#include <stdio.h>
#include <stdlib.h>

void run(void) {
	write("Good... Wait what?");
	system("/bin/sh");
}

int main(void) {
	char get[76];

	gets(get);
	return 0;
}
