/* 4.2 , 7.2
Q8) Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following commands.
myshell$ search a filename pattern :- To search all the occurrence of
pattern in the file.
myshell$ search c filename pattern :- To count the number of occurrence
of pattern in the file. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARG_LENGTH 100

void executeCommand(char *command) {
    char *args[MAX_ARG_LENGTH];
    char *token;
    int argCount = 0;

    // Tokenize the command input
    token = strtok(command, " \n");
    while (token != NULL && argCount < MAX_ARG_LENGTH) {
        args[argCount++] = token;
        token = strtok(NULL, " \n");
    }
    args[argCount] = NULL; // Null-terminate the array of arguments

    if (argCount > 0) {
        pid_t pid = fork(); // Create a child process

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            if (strcmp(args[0], "search") == 0) {
                if (argCount >= 3) {
                    char *filename = args[2];
                    char *pattern = args[3];

                    if (strcmp(args[1], "a") == 0) {
                        // Search for all occurrences of the pattern in the file
                        char command[MAX_CMD_LENGTH];
                        snprintf(command, sizeof(command), "grep -n '%s' '%s'", pattern, filename);
                        execlp("sh", "sh", "-c", command, NULL);
                    } else if (strcmp(args[1], "c") == 0) {
                        // Count the number of occurrences of the pattern in the file
                        char command[MAX_CMD_LENGTH];
                        snprintf(command, sizeof(command), "grep -c '%s' '%s'", pattern, filename);
                        execlp("sh", "sh", "-c", command, NULL);
                    } else {
                        fprintf(stderr, "Invalid search command\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Usage: search a|c filename pattern\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Unknown command: %s\n", args[0]);
                exit(EXIT_FAILURE);
            }
        } else { // Parent process
            wait(NULL); // Wait for the child process to finish
        }
    }
}

int main() {
    char command[MAX_CMD_LENGTH];

    while (1) {
        printf("myshell$ "); // Display the command prompt
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // Exit on EOF
        }
        executeCommand(command); // Execute the command
    }

    return 0;
}
