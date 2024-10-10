/*Write a programto implement the toy shell. It should display the command 
 prompt “myshell$”. Tokenize the command line and execute the given 
 command by creating the child process. Additionally it should interpret the 
 following commands. 
 count c filename :- To print number of characters in the file.
 count w filename :- To print number of words in the file.
 count l filename :- To print number of lines in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to count characters, words, and lines in a file
void count_file(char *filename, char mode) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int characters = 0, words = 0, lines = 0;
    char ch;
    char prev_ch = ' ';

    while ((ch = fgetc(file)) != EOF) {
        characters++; // Count characters

        if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (prev_ch != ' ' && prev_ch != '\n' && prev_ch != '\t') {
                words++;  // Count words when encountering a space or newline
            }
        }

        if (ch == '\n') {
            lines++;  // Count lines on encountering a newline
        }

        prev_ch = ch;
    }

    // If last word does not end with a space or newline, increment words
    if (prev_ch != ' ' && prev_ch != '\n' && prev_ch != '\t') {
        words++;
    }

    fclose(file);

    // Based on the mode, print the required count
    if (mode == 'c') {
        printf("Number of characters: %d\n", characters);
    } else if (mode == 'w') {
        printf("Number of words: %d\n", words);
    } else if (mode == 'l') {
        printf("Number of lines: %d\n", lines);
    } else {
        printf("Invalid mode\n");
    }
}

// Function to handle 'count' command
void handle_count(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: count [c|w|l] filename\n");
        return;
    }

    char mode = args[1][0]; // c for characters, w for words, l for lines
    count_file(args[2], mode);
}

// Function to execute external commands using fork and exec
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

// Function to tokenize and process user input
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

    // Exit the shell when user types 'exit'
    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // Handle the 'count' command internally
    if (strcmp(args[0], "count") == 0) {
        handle_count(args);
    } else {
        // If not 'count', execute the command normally
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

