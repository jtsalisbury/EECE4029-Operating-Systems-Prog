#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int printChildInfo(int loopCount) {
	int myPid = getpid();
	int myPPid = getppid();

	for (int i = 0; i < loopCount; i++) {
		printf("This is a child process. My PID is %i, my parent's PID is %i\n", myPid, myPPid);
		sleep(2);
	}

	return 1;	
}

int main(int argc, char *argv[]) {
	// Check to ensure the parameters are correct
	if (argc != 2 || !isdigit(*argv[1])) {
		printf("Usage: %s number-of-loops\n", argv[0]);
		exit(1);
	}

	// Get the number of loops we should go through
	int loopCount = *argv[1] - '0';

	for (int i = 0; i < 3; i++) {
		int pid = fork();
		if (pid == 0) {
			// Print the child info. Also, return so we don't keep making processes
			return printChildInfo(loopCount);
		}
	}

	for (int i = 0; i < loopCount; i++) {
		printf("This is the main process. My PID is %i\n", getpid());
		sleep(2);
	}
}
