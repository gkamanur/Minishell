#include <stdio.h>  // For printf
#include <stdlib.h> // For getenv, EXIT_SUCCESS
#include <unistd.h> // For extern char **environ

// environ is a global variable declared in <unistd.h>
// It's an array of strings, where each string is "NAME=VALUE".
extern char **environ;

int main() {
    // --- Using getenv() to get a specific variable ---
    char *path_value = getenv("PATH");
    if (path_value) {
        printf("Value of PATH: %s\n", path_value);
    } else {
        printf("PATH environment variable not found.\n");
    }

    char *home_value = getenv("HOME");
    if (home_value) {
        printf("Value of HOME: %s\n", home_value);
    } else {
        printf("HOME environment variable not found.\n");
    }

    char *non_existent_var = getenv("NON_EXISTENT_VAR");
    if (non_existent_var) {
        printf("Value of NON_EXISTENT_VAR: %s\n", non_existent_var);
    } else {
        printf("NON_EXISTENT_VAR environment variable not found (as expected).\n");
    }

    printf("\n--- Listing all environment variables using 'environ' ---\n");
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }

    return EXIT_SUCCESS;
}