/*Write a program to implement the shell. It should display the command 
 prompt “myshell$”. Tokenize the command line and execute the given 
 command by creating the child process. Additionally it should interpret the 
 following commands. 
 myshell$ search a filename pattern :- To search all the occurrence of 
 pattern in the file. 
 myshell$ search c filename pattern :- To count the number of occurrence 
 of pattern in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to search for all occurrences of a pattern in a file
void search_pattern(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_INPUT];
    int line_number = 1;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern) != NULL) {
            printf("Found pattern '%s' in line %d: %s", pattern, line_number, line);
            found = 1;
        }
        line_number++;
    }

    if (!found) {
        printf("No occurrences of pattern '%s' found in file '%s'.\n", pattern, filename);
    }

    fclose(file);
}

// Function to count occurrences of a pattern in a file
void count_pattern(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_INPUT];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *pos = line;
        while ((pos = strstr(pos, pattern)) != NULL) {
            count++;
            pos += strlen(pattern); // Move past the last found occurrence
        }
    }

    printf("Pattern '%s' occurs %d time(s) in file '%s'.\n", pattern, count, filename);

    fclose(file);
}

// Function to handle the 'search' command
void handle_search(char **args) {
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf("Usage: search a filename pattern or search c filename pattern\n");
        return;
    }

    if (strcmp(args[1], "a") == 0) {
        // search a filename pattern
        search_pattern(args[2], args[3]);
    } else if (strcmp(args[1], "c") == 0) {
        // search c filename pattern
        count_pattern(args[2], args[3]);
    } else {
        printf("Invalid option. Use 'a' for all occurrences or 'c' for count.\n");
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

    if (strcmp(args[0], "search") == 0) {
        handle_search(args);
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

