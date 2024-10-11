/*
 Write a program to implement the shell. It should display the command 
 prompt “myshell$”. Tokenize the command line and execute the given 
 command by creating the child process. Additionally it should interpret the 
 following commands. 
 myshell$ search f filename pattern :- To display first occurrence of 
 pattern in the file.
 myshell$ search c filename pattern :- To count the number of occurrence 
 of pattern in the file.  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to display the first occurrence of a pattern in a file
void search_first_occurrence(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int line_number = 1;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern) != NULL) {
            printf("First occurrence in line %d: %s", line_number, line);
            fclose(file);
            return;
        }
        line_number++;
    }

    printf("Pattern not found in the file.\n");
    fclose(file);
}

// Function to count the number of occurrences of a pattern in a file
void search_count_occurrences(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        char *pos = line;
        while ((pos = strstr(pos, pattern)) != NULL) {
            count++;
            pos++;
        }
    }

    printf("Number of occurrences: %d\n", count);
    fclose(file);
}

// Function to handle the 'search' command
void handle_search(char **args) {
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf("Usage: search f filename pattern or search c filename pattern\n");
        return;
    }

    if (strcmp(args[1], "f") == 0) {
        // search f filename pattern
        search_first_occurrence(args[2], args[3]);
    } else if (strcmp(args[1], "c") == 0) {
        // search c filename pattern
        search_count_occurrences(args[2], args[3]);
    } else {
        printf("Invalid option. Use 'f' or 'c'.\n");
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
        exit(0);
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

