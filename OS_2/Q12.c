/* 6.2, 9.2
Q12) Write a program to implement the shell. It should display the command prompt
“myshell$”. Tokenize the command line and execute the given command by
creating the child process. Additionally it should interpret the following
commands.
myshell$ search f filename pattern :- To display first occurrence of
pattern in the file.
myshell$ search a filename pattern :- To search all the occurrence of
pattern in the file. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

void execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    // Create a child process
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("myshell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WNOHANG);
        } while (wpid == 0); // Wait for the child process to finish
    }
}

void search_first_occurrence(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char line[MAX_INPUT_SIZE];
    int line_number = 1;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            printf("First occurrence of '%s' found in %s on line %d: %s", pattern, filename, line_number, line);
            fclose(file);
            return; // Exit after first occurrence
        }
        line_number++;
    }
    printf("Pattern '%s' not found in %s.\n", pattern, filename);
    fclose(file);
}

void search_all_occurrences(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char line[MAX_INPUT_SIZE];
    int line_number = 1;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            printf("Found '%s' in %s on line %d: %s", pattern, filename, line_number, line);
            found = 1;
        }
        line_number++;
    }

    if (!found) {
        printf("Pattern '%s' not found in %s.\n", pattern, filename);
    }
    fclose(file);
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell$ ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF
        }

        // Remove the trailing newline character
        input[strcspn(input, "\n")] = 0;

        // Tokenize the input
        char *token = strtok(input, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null terminate the argument list

        // Check for the search command
        if (i >= 4 && strcmp(args[0], "search") == 0) {
            if (strcmp(args[1], "f") == 0 && i == 4) {
                // Search for first occurrence
                search_first_occurrence(args[2], args[3]);
            } else if (strcmp(args[1], "a") == 0 && i == 4) {
                // Search for all occurrences
                search_all_occurrences(args[2], args[3]);
            } else {
                printf("Usage:\n");
                printf("myshell$ search f filename pattern\n");
                printf("myshell$ search a filename pattern\n");
            }
        } else {
            printf("Command not recognized.\n");
        }
    }

    return 0;
}
