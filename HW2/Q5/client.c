#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>

// Helper function to convert a string to upper case.
//   Loops through the characters of a string and makes them uppercase
void makeupper(char* str) {
	while (*str) {
		*str = toupper((unsigned char) *str);
		str++;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s file-to-read\n", argv[0]);
		exit(1);
	}

	const int FILESIZE = sizeof(char) + sizeof(int);
	const char FILENAME[] = "temp.bin";

	// Open a temporary file
	int fd = open(FILENAME, O_RDWR|O_CREAT|O_TRUNC, 0600);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	// Open the map!
	int* count = (int *)mmap(NULL, FILESIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
	if (count == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	// Set a reference to the pointer for the string
	char* str = (char *)count + sizeof(int);

	FILE* sourceFile = fopen(argv[1], "r");
	if (sourceFile == NULL) {
		perror(argv[1]);
		exit(1);
	}

	// Begin reading the lines from the source file
	char* line;
	size_t len = 0;

	while (getline(&line, &len, sourceFile) != -1) {
		makeupper(line); // convert to uppercase
		strcpy(str, line); //move the line to the memory for the string
		(*count)++; // update the count

		sleep(1);
	}

	// Send signal to the server to die
	strcpy(str, "Stop\n");
	(*count)++;

	// Detach segments and close files
	munmap(count, FILESIZE);
	close(fd);
	fclose(sourceFile);
}
