/*
 * Test program to check a process in /proc,
 * when given a process name for all the running the process,
 * if we don't know the process pid that we execute or executed.
 * It trys to read and check each of a list of pid with the /proc/pid/stat.
*/
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

static void usage(char *progname)
{
    fprintf(stderr, "usage: %s <process_name> \n", progname);
}

int main(int argc , char *argv[])
{
	DIR * dir;
	struct dirent * dirent;
    char sname[32], buf[512], process_name[32];
    int fd, len, p_count=0;
    unsigned pid;
    char *app, *progname;

    progname = strrchr(argv[0], '/');
    progname = progname ? 1+progname : argv[0];

    if(argc < 2) {
        usage(progname);
        return -1;
    }

    app = (char *)argv[1];

	if (!(dir = opendir("/proc"))) {
		perror("/proc directory could not be opened. ");
		return -1;
	}

	printf("\n Searching %s process ...\n\n", app);
    printf(" Found ==> \n");
    while ((dirent = readdir(dir))) {
        snprintf(sname, sizeof(sname), "/proc/%s/stat", dirent->d_name);
        if ((fd = open(sname, O_RDONLY)) < 0) {
            continue;
        }
        
        if ((len = read(fd, buf, sizeof(buf))) < 1 ) {
            close(fd);
            continue;
        }

        buf[len] = '\0';
        close(fd);

        // The start of the file looks like:
        //  <pid> (<name>) R <parent pid>
        if (sscanf(buf, "%u (%[^)]", &pid, process_name) != 2)
            continue;
        
        //printf("pid: %u, process name: %s \n", pid, process_name);
        if (strcmp(process_name, app) == 0) {
            p_count ++;
            printf("       pid: %u\n", pid);
        }
	}

    if(p_count)
        printf("\n ... %d %s process found \n\n", p_count, app);
    else
        printf("       None\n\n ...%s not running or none exist \n\n", app);

    closedir(dir);
    
    return 0;
}
