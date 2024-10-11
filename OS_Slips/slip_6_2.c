/*
 Write a programto implement the shell. It should display the command prompt 
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
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to search the first occurrence of a pattern in the file
void search_first_occurrence(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern) != NULL) {
            printf("First occurrence found at line %d: %s", lineNumber, line);
            fclose(file);
            return;
        }
        lineNumber++;
    }

    printf("Pattern '%s' not found in file '%s'.\n", pattern, filename);
    fclose(file);
}

// Function to search all occurrences of a pattern in the file
void search_all_occurrences(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int lineNumber = 1;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern) != NULL) {
            printf("Occurrence found at line %d: %s", lineNumber, line);
            found = 1;
        }
        lineNumber++;
    }

    if (!found) {
        printf("Pattern '%s' not found in file '%s'.\n", pattern, filename);
    }

    fclose(file);
}

// Function to handle 'search' command
void handle_search(char **args) {
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf("Usage: search <f/a> <filename> <pattern>\n");
        return;
    }

    if (strcmp(args[1], "f") == 0) {
        // search f filename pattern
        search_first_occurrence(args[2], args[3]);
    } else if (strcmp(args[1], "a") == 0) {
        // search a filename pattern
        search_all_occurrences(args[2], args[3]);
    } else {
        printf("Invalid option. Use 'f' for first occurrence or 'a' for all occurrences.\n");
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

// Function to tokenize and process the user input
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

    if (strcmp(args[0], "search") == 0) {
        handle_search(args);
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);  // Exit the shell
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

