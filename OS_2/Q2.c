/* 1.2
Q2) Write a C program to implement the shell which displays the command prompt “myshell$”. It accepts the command, tokenize the command line and execute it by creating the child process. Also implement the additional command ‘typeline’ as
typeline +n filename :- To print first n lines in the file.
typeline -a filename :- To print all lines in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

// Function to print the first n lines of a file
void print_first_n_lines(const char *filename, int n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL && line_count < n) {
        printf("%s", line);
        line_count++;
    }
    fclose(file);
}

// Function to print all lines of a file
void print_all_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
}

// Function to execute the 'typeline' command
void execute_typeline(char *args[]) {
    if (strcmp(args[1], "+n") == 0 && args[2] != NULL && args[3] != NULL) {
        int n = atoi(args[2]);
        print_first_n_lines(args[3], n);
    } else if (strcmp(args[1], "-a") == 0 && args[2] != NULL) {
        print_all_lines(args[2]);
    } else {
        printf("Invalid typeline command format\n");
        printf("Usage: typeline +n filename or typeline -a filename\n");
    }
}

// Function to handle and execute the command
void execute_command(char *args[]) {
    if (strcmp(args[0], "typeline") == 0) {
        execute_typeline(args);
    } else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent process waits for the child to finish
            wait(NULL);
        }
    }
}

// Function to tokenize the input command
void tokenize_command(char *command, char *args[]) {
    int i = 0;
    args[i] = strtok(command, " \n");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \n");
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        // Display the shell prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read the command from the user
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            perror("Error reading command");
            continue;
        }

        // Tokenize the command
        tokenize_command(command, args);

        // If no command entered, continue to next loop
        if (args[0] == NULL) {
            continue;
        }

        // Exit command to quit the shell
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        // Execute the command
        execute_command(args);
    }

    return 0;
}
