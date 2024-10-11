/* Write a programto implement the shell. It should display the command prompt 
 “myshell$”. Tokenize the command line and execute the given command by 
 creating the child process. Additionally it should interpret the following 
 commands. 
 myshell$ search f filename pattern :- To display first occurrence of 
 pattern in the file.
 myshell$ search c filename pattern :- To count the number of occurrence 
 of pattern in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to search for the first occurrence of a pattern in a file
void search_first_occurrence(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char line[256];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strstr(line, pattern) != NULL) {
            printf("First occurrence of pattern '%s' found in %s at line %d: %s",
                   pattern, filename, line_number, line);
            fclose(file);
            return;
        }
    }

    printf("Pattern '%s' not found in file '%s'.\n", pattern, filename);
    fclose(file);
}

// Function to count the occurrences of a pattern in a file
void count_occurrences(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        char *ptr = line;
        while ((ptr = strstr(ptr, pattern)) != NULL) {
            count++;
            ptr += strlen(pattern); // Move pointer past the last found occurrence
        }
    }

    printf("The pattern '%s' occurs %d times in the file '%s'.\n", pattern, count, filename);
    fclose(file);
}

// Main function to implement the shell
int main() {
    char command[256];

    while (1) {
        printf("myshell$ ");
        // Read the command from the user
        if (!fgets(command, sizeof(command), stdin)) {
            break; // Exit on EOF
        }

        // Remove the newline character at the end of the command
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command
        char *token = strtok(command, " ");
        if (token == NULL) {
            continue; // Empty command
        }

        // Check for the 'search' command
        if (strcmp(token, "search") == 0) {
            token = strtok(NULL, " "); // Get the next token
            if (token == NULL) {
                printf("Invalid command format.\n");
                continue;
            }

            char *filename = strtok(NULL, " "); // Get filename
            char *pattern = strtok(NULL, " "); // Get pattern
            if (filename == NULL || pattern == NULL) {
                printf("Invalid command format. Usage: search <f/c> <filename> <pattern>\n");
                continue;
            }

            // Fork a child process to execute the command
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                continue;
            }

            if (pid == 0) { // Child process
                if (strcmp(token, "f") == 0) {
                    search_first_occurrence(filename, pattern);
                } else if (strcmp(token, "c") == 0) {
                    count_occurrences(filename, pattern);
                } else {
                    printf("Invalid option. Use 'f' for first occurrence or 'c' for count.\n");
                }
                exit(0); // Terminate child process
            } else { // Parent process
                wait(NULL); // Wait for child to finish
            }
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}

