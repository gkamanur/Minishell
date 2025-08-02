#include <stdio.h>    // For printf, perror
#include <stdlib.h>   // For exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>   // For fork, execve, waitpid
#include <sys/wait.h> // For waitpid, WIFEXITED, WEXITSTATUS

int main() {
    pid_t pid;
    int status;
    char *args[] = {"/bin/ls", "-l", "/tmp", NULL}; // Arguments for ls -l /tmp
    char *envp[] = {NULL}; // Environment variables for the new process (can be inherited from parent if NULL)

    printf("Parent process (PID: %d) is about to fork.\n", getpid());

    // 1. Create a child process using fork()
    pid = fork();

    if (pid == -1) {
        // fork() failed
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // This code runs in the child process
        printf("Child process (PID: %d) created. Executing command...\n", getpid());

        // 2. Execute the command in the child process using execve()
        //    execve() replaces the current process image with a new one.
        //    It takes:
        //    - path: The path to the executable (e.g., "/bin/ls").
        //    - argv: An array of strings representing the arguments.
        //            The first argument (argv[0]) should be the name of the executable.
        //            The array must be NULL-terminated.
        //    - envp: An array of strings representing the environment variables.
        //            This array must also be NULL-terminated. If you want to inherit
        //            the parent's environment, you can pass `environ` (from unistd.h)
        //            or construct your own. For simplicity, we pass NULL here.
        if (execve(args[0], args, envp) == -1) {
            // execve() only returns if an error occurred.
            // If successful, the code after execve() is never reached.
            perror("execve failed");
            exit(EXIT_FAILURE); // Child exits on execve failure
        }
    } else {
        // This code runs in the parent process
        printf("Parent process (PID: %d) waiting for child (PID: %d) to complete...\n", getpid(), pid);

        // 3. Parent waits for the child process to terminate using waitpid()
        //    - pid: The PID of the child to wait for (or -1 to wait for any child).
        //    - &status: A pointer to an integer where the child's exit status will be stored.
        //    - options: Flags (e.g., WNOHANG for non-blocking). 0 means blocking.
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        // 4. Check the child's exit status
        if (WIFEXITED(status)) {
            // WIFEXITED checks if the child terminated normally
            printf("Child process (PID: %d) exited with status %d.\n", pid, WEXITSTATUS(status));
            // WEXITSTATUS retrieves the exit status code
        } else {
            printf("Child process (PID: %d) terminated abnormally.\n", pid);
        }
    }

    printf("Parent process (PID: %d) finished.\n", getpid());
    return EXIT_SUCCESS;
}