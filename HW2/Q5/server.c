#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main() {
	const int FILESIZE = sizeof(char) + sizeof(int);
	const char FILENAME[] = "temp.bin";

	// Open a temporary file
	int fd = open(FILENAME, O_RDWR|O_CREAT|O_TRUNC, 0600);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	// Expand the filesize 
	int result = lseek(fd, FILESIZE - 1, SEEK_SET);
	if (result == -1) {
		perror("lseek");
		exit(1);
	}

	// Write a dumby bit to actually expand the size
	result = write(fd, "", 1);
	if (result != 1) {
		perror("write");
		exit(1);
	}

	// Open the memory segment
	int* count = (int *)mmap(NULL, FILESIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (count == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	// Set a reference to the pointer for the string
	char* str = (char *)count + sizeof(int);

	// Begin reading the segments
	int old = *count;
	while (1) {
		// If the integer count changes
		if (old != *count) {
			old = *count; // update the count

			printf("%s", str); // print the string

			// If it's a stop, then let's get out of the loop
			if (strcmp(str, "Stop\n") == 0) {
				break;
			}
		}
	}

	// Delete the memory segment, close & delete the temp file
	munmap(count, FILESIZE);
	close(fd);
	remove(FILENAME);
}
