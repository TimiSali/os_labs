#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	int pid = fork();

	if ( pid == 0 ) {
		execvp( "./sequential_min_max", argv);
        exit(0);
	}

	/* Put the parent to sleep until the child finished executing */
	wait( NULL );

	return 0;
}