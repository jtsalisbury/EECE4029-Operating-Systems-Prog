#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

int main() {
	const int SHM_SIZE = 1024;
	const char FILENAME_STR[] = "shm.dat";
	const char FILENAME_COUNT[] = "shm2.dat";

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

	// Delete the memory segments
	shmctl(shmidstr, IPC_RMID, NULL);
	shmctl(shmidcount, IPC_RMID, NULL);
}
