/*
 * Test program to check the status of module driver using /proc/modules file.
 * It returns 1 if dirver module exists in /proc/modules
 * or returns 0 if none exists in /proc/modules.
 * It may be used to call to check the driver status after rmmod.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULE_FILE "/proc/modules"

int main(int argc, char* argv[])
{
	const char module_file[] = MODULE_FILE;
	char *driver_module_tag;
	char line[sizeof(argv[1])+10];
	FILE *proc;

    if(argc != 2 ){
            printf("Usage: check_module <module name>\n");
            return 0;
    }

    driver_module_tag = malloc(strlen((char *)argv[1])+1);
    strcpy(driver_module_tag, argv[1]);
    strcpy(driver_module_tag + strlen(argv[1])," ");

	if((proc = fopen(module_file, "r")) == NULL) {
		printf("[%s] Could not open %s\n", __func__, module_file);
		return -1;
	}
	while ((fgets(line, sizeof(line), proc)) != NULL) {
		printf("[%s] line: %s\n", __func__, line);
        if (strncmp(line, driver_module_tag, strlen(driver_module_tag)) == 0) {
                fclose(proc);
                printf("[%s] %s driver module founded in /proc/modules\n", __func__, driver_module_tag);
                return 1;
        }
	}
    free(driver_module_tag);
	fclose(proc);
	return 0;
}
