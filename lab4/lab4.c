#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SHM_NAME "/lab4.mem"

int main(int argc, char *argv[], char *envp[]) {
    printf("Enter input filename: ");

    char filename[FILENAME_MAX];
    scanf("%s", filename);

    int input_fd = open(filename, O_RDONLY); // input file
    if (input_fd < 0) {
        fprintf(stderr, "Error opening input file: %s\n", strerror(errno));
        return -1;
    }

    shm_unlink(SHM_NAME);  // cleanup shared memory name
    // give name to shared memory
    int map_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
    if (map_fd < 0) {
        fprintf(stderr, "Error opening shared memory: %s\n", strerror(errno));
        return -1;
    }

    ftruncate(map_fd, sizeof(int)); // truncate to size of 1 int

    // get address of named shared memory
    char *addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "Error create mapping: %s\n", strerror(errno));
        return -1;
    }

    int child_pid = fork();
    if (child_pid < 0) {
        fprintf(stderr, "Error forking: %s\n", strerror(errno));
        return -1;
    }

    if (child_pid != 0) {
        // parent process
        if (waitpid(child_pid, NULL, 0) < 0) {
            fprintf(stderr, "Error waiting for child processes: %s\n", strerror(errno));
            return -1;
        }

        // get result from shared memory
        int result = *addr;
        printf("%d\n", result);
    } else {
        // child process
        dup2(input_fd, STDIN_FILENO); // substitute standard input with input file
        execve("lab4child", argv, envp);
    }
    return 0;
}
