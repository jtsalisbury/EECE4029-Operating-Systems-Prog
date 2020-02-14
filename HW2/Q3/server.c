#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
	// Open the fifo
	char* fifo = "./fifo";
	mkfifo(fifo, 0666);

	int fd = open(fifo, O_RDONLY);
	char buffer[100];

	// Begin waiting for inputs
	while (1) {
		read(fd, buffer, sizeof(buffer)); // read into the buffer
		printf("%s", buffer); // print the read string
		if (strcmp(buffer, "Stop\n") == 0) { // see if we should stop
			break;
		}
	}

	// Close & delete the fifo
	close(fd);
	unlink("fifo");
}
