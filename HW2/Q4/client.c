#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/shm.h>

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

	const int SHM_SIZE = 1024;
	const char FILENAME_STR[] = "shm.dat";
	const char FILENAME_COUNT[] = "shm2.dat";

	// Note: Two memory segments are created. 1 to hold the string and 1 to hold the count. 
	// I tried doing both with pointer casting and incrememnting (one ptr as char* and one as int*, but came up short everytime).

	// Generate a key
	key_t keystr = ftok(FILENAME_STR, 1);
	if (keystr == -1) {
		perror("ftok");
		exit(1);
	}

	key_t keycount = ftok(FILENAME_COUNT, 1);
	if (keycount == -1) {
		perror("ftok");
		exit(1);
	}

	// Connect to and create the shared memory space
	int shmidstr = shmget(keystr, SHM_SIZE, 0644|IPC_CREAT);
	if (shmidstr == -1) {
		perror("shmget");
		exit(1);
	}

	int shmidcount = shmget(keycount, SHM_SIZE, 0644|IPC_CREAT);
	if (shmidcount == -1) {
		perror("shmget");
		exit(1);
	}

	// Attach to memory segment
	char* str = (char *)shmat(shmidstr, (void *)0, 0);
	if (str == (char *)-1) {
		perror("shmat");
		exit(1);
	}

	int* count = (int *)shmat(shmidcount, (void *)0, 0);
	if (count == (int *)-1) {
		perror("shmat");
		exit(1);
	}
	*(count) = 0;

	// Open the text file
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

	// Detach segments
	shmdt(str);
	shmdt(count);
}
