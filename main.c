#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h> 

#define MEM_THRESHOLD 5 * 1000  // 5GB in MB

void check_process_memory_and_kill() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir(/proc)");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char *endptr;
            long pid = strtol(entry->d_name, &endptr, 10);

            if (*endptr == '\0') {  // If the directory name is a number
                char status_path[256];
                snprintf(status_path, sizeof(status_path), "/proc/%ld/status", pid);

                FILE *status_file = fopen(status_path, "r");
                if (status_file != NULL) {
                    char line[256];
                    long vmrss = 0;

                    while (fgets(line, sizeof(line), status_file)) {
                        if (strncmp(line, "VmRSS:", 6) == 0) {
                            sscanf(line + 6, "%ld", &vmrss);
                            break;
                        }
                    }

                    fclose(status_file);

                    if (vmrss > MEM_THRESHOLD * 1024) {
                        printf("Killing process %ld (VmRSS: %ld kB)\n", pid, vmrss);
                        if (kill(pid, SIGKILL) == -1) {
                            perror("kill");
                        }
                    }
                }
            }
        }
    }

    closedir(dir);
}

int main() {
    while (1) {
        check_process_memory_and_kill();
        sleep(60 * 1);  // Check every 1 minute
    }

    return 0;
}
