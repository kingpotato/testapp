/*
 * User space test program: 
 * Check a sepecific file or directory exist
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/poll.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	char *file_path;

    if(argc != 2) {
        printf("Usage: check-file <file_path>\n");
        exit(0);
    }

    file_path = (char *)argv[1];

	if(access(file_path, F_OK) == -1) {
		printf("Cannot access: %s\n", file_path);
		return -1;
	}
	printf("Exist: %s\n", file_path);
	return 0;
}
