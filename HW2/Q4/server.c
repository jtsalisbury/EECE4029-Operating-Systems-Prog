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
	const char FILENAME[] = "shm.dat";

	// Generate a key
	key_t key = ftok(FILENAME, 1);
	if (key == -1) {
		perror("ftok");
		exit(1);
	}

	// Connect to and create the shared memory space
	int shmid = shmget(key, SHM_SIZE, 0644|IPC_CREAT);
	if (shmid == -1) {
		perror("shmget");
		exit(1);
	}

	// Attach to memory segment
	int* count = (int *)shmat(shmid, (void *)0, 0);
	if (count == (int *)-1) {
		perror("shmat");
		exit(1);
	}

	// Reference to the string
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

	// Delete the memory segments
	shmctl(shmid, IPC_RMID, NULL);
}
