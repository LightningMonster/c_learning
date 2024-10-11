/* 13.1, 20.1
Q15) Write a C program to implement the shell which displays the command
prompt “myshell$”. It accepts the command, tokenize the command line and
execute it by creating the child process. Also implement the additional
command ‘typeline’ as
typeline -a filename :- To print all lines in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024

// Function to tokenize the command input
void tokenizeInput(char *input, char *args[], int *argCount) {
    char *token = strtok(input, " \n");
    *argCount = 0;
    while (token != NULL) {
        args[(*argCount)++] = token;
        token = strtok(NULL, " \n");
    }
    args[*argCount] = NULL;
}

// Function to handle the custom 'typeline -a filename' command
void typelineAll(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_INPUT];
    int argCount;

    while (1) {
        printf("myshell$ ");
        fgets(input, MAX_INPUT, stdin);

        // Tokenize the input
        tokenizeInput(input, args, &argCount);

        // Exit command to terminate the shell
        if (argCount > 0 && strcmp(args[0], "exit") == 0) {
            break;
        }

        // Handle the custom 'typeline -a filename' command
        if (argCount == 3 && strcmp(args[0], "typeline") == 0 && strcmp(args[1], "-a") == 0) {
            typelineAll(args[2]);
        } else {
            // Fork a child process for executing other commands
            pid_t pid = fork();

            if (pid < 0) {
                perror("Fork failed");
            } else if (pid == 0) {
                // Child process
                if (execvp(args[0], args) < 0) {
                    perror("Command execution failed");
                }
                exit(0);
            } else {
                // Parent process waits for the child to complete
                wait(NULL);
            }
        }
    }

    return 0;
}
