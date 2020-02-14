#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	// Ensure the correct arguments are supplied
	if (argc != 2) {
		printf("Usage: %s file-name", argv[0]);
		exit(1);
	}
	
	// Begin forking the children processes
	for (int i = 0; i < 3; i++) {
		int pid = fork();
	
		if (pid == 0) {
			if (i == 0) {
				// Print the files
				execl("/bin/ls", "ls", "-l", (char*)0);
			} else if (i == 1) {
				// Print the processes
				execl("/bin/ps", "ps", "-ef", (char*)0);		
			} else {
				// Print the contents of the file
				execl("/bin/cat", "cat", argv[1], (char*)0);
			}	
			
			// Return so that we don't keep making processes
			return 1;
		}
	
		// Make the parent wait for each child to finish
		wait(NULL);
	}
	
	// Print the main process terminat	
	// // Print the main process terminatinging
	printf("main process terminates\n");
}
