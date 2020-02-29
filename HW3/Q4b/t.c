#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

int main() {
	int c;
	c = fork();
	printf("process\n");
	c = fork();
	printf("process\n");
	if (c == 0) {
		fork();
		printf("process\n");
	}
}
