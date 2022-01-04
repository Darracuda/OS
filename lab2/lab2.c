#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    int input_fd = open("test.txt", O_RDONLY); // input file
    if (input_fd < 0){
        printf("Error opening input file\n");
        return -1;
    }

    int pipe_fd[2]; // 2 descriptors, 0 for reading from pipe, 1 for writing to pipe
    if (pipe(pipe_fd) != 0) {
        printf("Error piping\n");
        return -1;
    }

    int child_pid = fork();
    if (child_pid < 0) {
        printf("Error forking\n");
        return -1;
    }

    if (child_pid != 0) {
        // parent process
        close(pipe_fd[1]); // close pipe's write gate
        dup2(pipe_fd[0], STDIN_FILENO); // substitute standard input with pipe's read gate
        int result = 0;
        scanf("%d", &result);
        printf("%d\n", result);
    } else {
        // child process
        close(pipe_fd[0]); // close pipe's read gate
        dup2(input_fd, STDIN_FILENO); // substitute standard input with input file
        dup2(pipe_fd[1], STDOUT_FILENO); // substitute standard output with pipe's write gate
        execve("lab2child", argv, envp);
    }
    return 0;
}
