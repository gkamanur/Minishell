#include <stdio.h>    // For printf, perror
#include <stdlib.h>   // For exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>   // For fork, execve, dup2, close, STDIN_FILENO, STDOUT_FILENO
#include <fcntl.h>    // For open, O_RDONLY, O_WRONLY, O_CREAT, O_APPEND
#include <sys/wait.h> // For waitpid, WIFEXITED, WEXITSTATUS

void create_test_files() {
    FILE *in_file = fopen("input.txt", "w");
    if (in_file) {
        fprintf(in_file, "Line 1 from input\n");
        fprintf(in_file, "Line 2 from input\n");
        fclose(in_file);
        printf("Created 'input.txt' for testing input redirection.\n");
    } else {
        perror("Failed to create input.txt");
    }

    FILE *out_file = fopen("output.txt", "w"); // Create an empty file to be overwritten
    if (out_file) {
        fprintf(out_file, "This content will be overwritten.\n");
        fclose(out_file);
        printf("Created 'output.txt' (will be overwritten/appended).\n");
    } else {
        perror("Failed to create output.txt");
    }
}

int main() {
    pid_t pid;
    int status;
    char *envp[] = {NULL}; // Environment variables for simplicity

    create_test_files(); // Create files needed for demonstration

    printf("\n--- Demonstrating Input Redirection (<) ---\n");
    // Example: cat < input.txt
    pid = fork();
    if (pid == -1) { perror("fork failed"); exit(EXIT_FAILURE); }
    if (pid == 0) {
        int fd_in = open("input.txt", O_RDONLY);
        if (fd_in == -1) { perror("open input.txt failed"); exit(EXIT_FAILURE); }

        // Redirect stdin (fd 0) to input.txt
        if (dup2(fd_in, STDIN_FILENO) == -1) { perror("dup2 stdin failed"); exit(EXIT_FAILURE); }
        close(fd_in); // Close the original file descriptor after duplication

        char *cmd_args[] = {"/bin/cat", NULL};
        printf("Child (PID: %d) executing 'cat < input.txt'\n", getpid());
        if (execve(cmd_args[0], cmd_args, envp) == -1) { perror("execve cat failed"); exit(EXIT_FAILURE); }
    }
    waitpid(pid, &status, 0);
    printf("Parent: Input redirection example finished.\n");

    printf("\n--- Demonstrating Output Redirection (>) (Overwrite) ---\n");
    // Example: echo "Hello World" > output.txt
    pid = fork();
    if (pid == -1) { perror("fork failed"); exit(EXIT_FAILURE); }
    if (pid == 0) {
        // Open output.txt for writing, create if not exists, truncate (overwrite) if it does.
        int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1) { perror("open output.txt failed"); exit(EXIT_FAILURE); }

        // Redirect stdout (fd 1) to output.txt
        if (dup2(fd_out, STDOUT_FILENO) == -1) { perror("dup2 stdout failed"); exit(EXIT_FAILURE); }
        close(fd_out); // Close the original file descriptor

        char *cmd_args[] = {"/bin/echo", "Hello World from overwrite", NULL};
        printf("Child (PID: %d) executing 'echo \"Hello World\" > output.txt'\n", getpid());
        if (execve(cmd_args[0], cmd_args, envp) == -1) { perror("execve echo failed"); exit(EXIT_FAILURE); }
    }
    waitpid(pid, &status, 0);
    printf("Parent: Output overwrite example finished. Check 'output.txt'.\n");

    printf("\n--- Demonstrating Output Redirection (>>) (Append) ---\n");
    // Example: echo "Appended Line" >> output.txt
    pid = fork();
    if (pid == -1) { perror("fork failed"); exit(EXIT_FAILURE); }
    if (pid == 0) {
        // Open output.txt for writing, create if not exists, append if it does.
        int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out == -1) { perror("open output.txt failed"); exit(EXIT_FAILURE); }

        // Redirect stdout (fd 1) to output.txt
        if (dup2(fd_out, STDOUT_FILENO) == -1) { perror("dup2 stdout failed"); exit(EXIT_FAILURE); }
        close(fd_out); // Close the original file descriptor

        char *cmd_args[] = {"/bin/echo", "Appended Line from append", NULL};
        printf("Child (PID: %d) executing 'echo \"Appended Line\" >> output.txt'\n", getpid());
        if (execve(cmd_args[0], cmd_args, envp) == -1) { perror("execve echo failed"); exit(EXIT_FAILURE); }
    }
    waitpid(pid, &status, 0);
    printf("Parent: Output append example finished. Check 'output.txt'.\n");

    // Clean up test files (optional, but good practice)
    // unlink("input.txt");
    // unlink("output.txt");

    return EXIT_SUCCESS;
}
