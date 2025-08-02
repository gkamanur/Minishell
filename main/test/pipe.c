#include <stdio.h>    // For printf, perror
#include <stdlib.h>   // For exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>   // For pipe, fork, dup2, close, execve
#include <sys/wait.h> // For waitpid, WIFEXITED, WEXITSTATUS

int main() {
    int pipefd[2]; // pipefd[0] for read end, pipefd[1] for write end
    pid_t pid1, pid2;
    int status1, status2;

    // Arguments for the first command: ls -l
    char *cmd1_args[] = {"/bin/ls", "-l", NULL};
    // Arguments for the second command: wc -l
    char *cmd2_args[] = {"/usr/bin/wc", "-l", NULL};
    char *envp[] = {NULL}; // Environment variables for simplicity

    printf("Parent process (PID: %d) is setting up a pipe.\n", getpid());

    // 1. Create a pipe
    //    pipefd[0] will be the read end of the pipe.
    //    pipefd[1] will be the write end of the pipe.
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // 2. Fork the first child process (for the 'ls -l' command)
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork (cmd1) failed");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // This is the first child process (for 'ls -l')
        printf("Child 1 (PID: %d) created. Will execute 'ls -l'.\n", getpid());

        // Close the read end of the pipe in this child, as it will only write.
        close(pipefd[0]);

        // Redirect standard output (stdout, file descriptor 1) to the write end of the pipe.
        // After this, anything written to stdout by 'ls -l' will go into the pipe.
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 (cmd1 stdout) failed");
            exit(EXIT_FAILURE);
        }
        // Close the original write end of the pipe after dup2, as it's no longer needed.
        close(pipefd[1]);

        // Execute the first command
        if (execve(cmd1_args[0], cmd1_args, envp) == -1) {
            perror("execve (cmd1) failed");
            exit(EXIT_FAILURE);
        }
    }

    // 3. Fork the second child process (for the 'wc -l' command)
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork (cmd2) failed");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // This is the second child process (for 'wc -l')
        printf("Child 2 (PID: %d) created. Will execute 'wc -l'.\n", getpid());

        // Close the write end of the pipe in this child, as it will only read.
        close(pipefd[1]);

        // Redirect standard input (stdin, file descriptor 0) to the read end of the pipe.
        // After this, 'wc -l' will read its input from the pipe.
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 (cmd2 stdin) failed");
            exit(EXIT_FAILURE);
        }
        // Close the original read end of the pipe after dup2.
        close(pipefd[0]);

        // Execute the second command
        if (execve(cmd2_args[0], cmd2_args, envp) == -1) {
            perror("execve (cmd2) failed");
            exit(EXIT_FAILURE);
        }
    }

    // This code runs in the parent process
    printf("Parent process (PID: %d) is closing pipe ends and waiting for children.\n", getpid());

    // 4. Parent closes both ends of the pipe.
    //    It's crucial for the parent to close its copies of the pipe file descriptors.
    //    If the parent keeps the write end open, the second child (wc -l) might
    //    never receive an EOF, causing it to hang if the first child (ls -l) finishes
    //    before the parent closes its write end.
    close(pipefd[0]);
    close(pipefd[1]);

    // 5. Parent waits for both child processes to terminate
    printf("Parent waiting for Child 1 (PID: %d)...\n", pid1);
    if (waitpid(pid1, &status1, 0) == -1) {
        perror("waitpid (cmd1) failed");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status1)) {
        printf("Child 1 (PID: %d) exited with status %d.\n", pid1, WEXITSTATUS(status1));
    } else {
        printf("Child 1 (PID: %d) terminated abnormally.\n", pid1);
    }

    printf("Parent waiting for Child 2 (PID: %d)...\n", pid2);
    if (waitpid(pid2, &status2, 0) == -1) {
        perror("waitpid (cmd2) failed");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status2)) {
        printf("Child 2 (PID: %d) exited with status %d.\n", pid2, WEXITSTATUS(status2));
    } else {
        printf("Child 2 (PID: %d) terminated abnormally.\n", pid2);
    }

    printf("Parent process (PID: %d) finished.\n", getpid());
    return EXIT_SUCCESS;
}
