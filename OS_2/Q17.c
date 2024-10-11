/* 14.1
Q17) Write a C program to implement the shell which displays the command
prompt “myshell$”. It accepts the command, tokenize the command line and
execute it by creating the child process. Also implement the additional
command ‘typeline’ as
typeline +n filename :- To print first n lines in the file. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 100
#define MAX_ARGS 10

// Function to tokenize the command line input
void tokenize(char *input, char **args) {
    for (int i = 0; i < MAX_ARGS; i++) {
        args[i] = strsep(&input, " ");
        if (args[i] == NULL) break;
    }
}

// Function to handle the 'typeline' command
void typeline(char *filename, int n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL && count < n) {
        printf("%s", line);
        count++;
    }

    fclose(file);
}

// Function to execute the command
void executeCommand(char **args) {
    if (strcmp(args[0], "typeline") == 0 && args[1] != NULL && args[2] != NULL) {
        if (args[1][0] == '+') {
            int n = atoi(args[1] + 1);  // Convert the number after '+' to an integer
            typeline(args[2], n);       // Call the typeline function
        } else {
            printf("Invalid typeline command. Use typeline +n filename.\n");
        }
    } else {
        pid_t pid = fork();  // Create a child process
        if (pid == 0) {
            // In child process
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
            }
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // In parent process, wait for the child to complete
            wait(NULL);
        } else {
            perror("Fork failed");
        }
    }
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        // Display shell prompt
        printf("myshell$ ");
        fgets(input, sizeof(input), stdin);

        // Remove the newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the command
        tokenize(input, args);

        // Exit the shell if the command is "exit"
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        // Execute the command
        executeCommand(args);
    }

    return 0;
}
