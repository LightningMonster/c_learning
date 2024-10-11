/* Write a program to implement shell. It should display the command prompt 
 “myshell$”. Tokenize the command line and execute the given command by 
 creating the child process. Additionally it should interpret the following 
 commands. 
 myshell$ search a filename pattern :- To search all the occurrence of 
 pattern in the file. 
 myshell$ search c filename pattern :- To count the number of occurrence 
 of pattern in the file. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

// Function to search all occurrences of a pattern in a file
void searchAllOccurrences(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[1024];
    int lineNumber = 1;
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, pattern) != NULL) {
            printf("Found in line %d: %s", lineNumber, line);
            found = 1;
        }
        lineNumber++;
    }

    if (!found) {
        printf("Pattern '%s' not found in file %s\n", pattern, filename);
    }

    fclose(file);
}

// Function to count occurrences of a pattern in a file
void countOccurrences(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[1024];
    int lineNumber = 1;
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *pos = line;
        while ((pos = strstr(pos, pattern)) != NULL) {
            count++;
            pos += strlen(pattern); // Move past the pattern
        }
        lineNumber++;
    }

    printf("Total occurrences of '%s' in file %s: %d\n", pattern, filename, count);

    fclose(file);
}

// Main shell loop
void shell() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        // Display prompt
        printf("myshell$ ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Remove newline character from command
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command
        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // If the command is "exit", break the loop
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        // If the command is "search", handle it
        if (strcmp(args[0], "search") == 0 && i == 4) {
            char *operation = args[1];
            char *filename = args[2];
            char *pattern = args[3];

            if (strcmp(operation, "a") == 0) {
                searchAllOccurrences(filename, pattern);
            } else if (strcmp(operation, "c") == 0) {
                countOccurrences(filename, pattern);
            } else {
                printf("Invalid operation. Use 'a' for all occurrences or 'c' for counting.\n");
            }
        } else {
            // For other commands, create a child process and execute the command
            pid_t pid = fork();

            if (pid == 0) {
                // Child process
                if (execvp(args[0], args) == -1) {
                    printf("Error: Command not found.\n");
                }
                exit(0);
            } else if (pid > 0) {
                // Parent process, wait for the child to finish
                wait(NULL);
            } else {
                printf("Error: Fork failed.\n");
            }
        }
    }
}

int main() {
    shell();
    return 0;
}

