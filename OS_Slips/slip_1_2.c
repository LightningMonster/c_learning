/*
Q.2 Write a C program to implement the shell which displays the command prompt “myshell$”. It accepts the command, tokenize the command line and execute it by creating the child process. Also implement the additional command 
‘typeline’ as  typeline +n   filename      :- To print first n lines in the file. typeline -a    filename      :- To print all  lines in the file. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to print first n lines from the file
void print_first_n_lines(char *filename, int n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        if (++count == n) {
            break;
        }
    }

    fclose(file);
}

// Function to print all lines from the file
void print_all_lines(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Function to handle 'typeline' command
void handle_typeline(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: typeline +n filename or typeline -a filename\n");
        return;
    }

    if (args[1][0] == '+') {
        // typeline +n filename
        int n = atoi(&args[1][1]);
        print_first_n_lines(args[2], n);
    } else if (strcmp(args[1], "-a") == 0) {
        // typeline -a filename
        print_all_lines(args[2]);
    } else {
        printf("Invalid option. Use +n or -a\n");
    }
}

// Function to execute external commands
void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
        }
        exit(EXIT_FAILURE);
    } else {
        // Parent process, wait for the child to finish
        wait(NULL);
    }
}

// Function to tokenize and execute the user input
void process_input(char *input) {
    char *args[MAX_ARGS];
    char *token = strtok(input, " \n");

    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;  // Null-terminate the argument list

    if (args[0] == NULL) {
        return;  // No command entered
    }

    if (strcmp(args[0], "typeline") == 0) {
        handle_typeline(args);
    } else {
        execute_command(args);
    }
}

int main() {
    char input[MAX_INPUT];

    while (1) {
        // Display the prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read input from user
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        // Process the input
        process_input(input);
    }

    return 0;
}
