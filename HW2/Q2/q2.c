#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
	int fdFind[2];
	int fdTz[2];

	const int BUFFSIZE = 100;
	char buffer[BUFFSIZE];

	// Open pipes to find all files and write to tr
	FILE *fp = popen("find . -name '*.c'", "r");
	FILE *fp2 = popen("tr a-z A-Z", "w");

	// While we are still receiving stuff from find, write the string to the tr pipe
	// This by default will use the stdout of the terminal window
	while (fgets(buffer, BUFFSIZE, fp) != NULL) {
		fputs(&buffer[0], fp2);
	}

	// Close the pipes!
	pclose(fp);
	pclose(fp2);
}

