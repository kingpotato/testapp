/*
 * User space test program:
 * To execute a script file
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

static int execute_sh_file(const char *file_path, char *const args[],
		char *const env[])
{
	pid_t pid = 0; 
	int rv = 0; 

	if (!(pid = fork())) {
		register unsigned int index = 0; 
		printf("Inside child, exec (%s) command\n", file_path);

		index = 0; 
		while (args[index] != NULL) {
			index++;
		} 

		if (execve(file_path, args, env) == -1) {
			printf("fail to execute commnad...(%s)\n", file_path);
			return -1;
		}    
	} else if (pid > 0) { 

		if (waitpid(pid, &rv, 0) == -1) {
			printf("wait pid (%u) rv (%d)", pid, rv); 

			if (WIFEXITED(rv)) {
				printf("exited, rv=%d\n", WEXITSTATUS(rv));
			} else if (WIFSIGNALED(rv)) {
				printf("killed by signal %d\n", WTERMSIG(rv));
			} else if (WIFSTOPPED(rv)) {
				printf("stopped by signal %d\n", WSTOPSIG(rv));
			} else if (WIFCONTINUED(rv)) {
				printf("continued\n");
			}    
		}
		return 0;
	}

	printf("failed to fork()...\n");
	return -1;
}


int main()
{
	{
		printf("%s: enter\n", __func__);
		const char *path = "/root/test.sh";
		char *const args[] = { "test.sh", "start", NULL };
		char *const envs[] = { NULL };
	
		if (execute_sh_file(path, args, envs)) {
			printf("[%s]: can't execute script file\n", __func__);
			return false;
		}
		return true;
	}
}
