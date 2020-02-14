#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s program-to-run file-to-output\n", argv[0]);
		exit(1);
	}

	int pid = fork();
	if (pid == 0) {
		// We are the child process!
		int fd = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

		// Send contents of stdout to file
		dup2(fd, 1);
		
		// Close file since we are sending the output already
		close(fd);

		execl(argv[1], argv[1], (char*)0);
	}
}
