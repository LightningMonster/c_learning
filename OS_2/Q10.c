/* 5.2, 8.2
Q10) Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following commands.
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

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 100

void search_first_occurrence(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char line[MAX_CMD_LENGTH];
    int line_number = 0;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strstr(line, pattern)) {
            printf("First occurrence of '%s' found in %s at line %d: %s", pattern, filename, line_number, line);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Pattern '%s' not found in %s\n", pattern, filename);
    }

    fclose(file);
}

void search_count_occurrences(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char line[MAX_CMD_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char *ptr = line;
        while ((ptr = strstr(ptr, pattern)) != NULL) {
            count++;
            ptr += strlen(pattern); // Move past the last found occurrence
        }
    }

    printf("The pattern '%s' occurs %d times in %s\n", pattern, count, filename);

    fclose(file);
}

int main() {
    char command[MAX_CMD_LENGTH];

    while (1) {
        printf("myshell$ ");
        if (!fgets(command, sizeof(command), stdin)) {
            break; // Exit on EOF
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Tokenize the command
        char *args[MAX_ARGS];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the argument list

        // Check for the 'search' command
        if (strcmp(args[0], "search") == 0 && i >= 4) {
            char *type = args[1];
            char *filename = args[2];
            char *pattern = args[3];

            pid_t pid = fork();
            if (pid == 0) { // Child process
                if (strcmp(type, "f") == 0) {
                    search_first_occurrence(filename, pattern);
                } else if (strcmp(type, "c") == 0) {
                    search_count_occurrences(filename, pattern);
                } else {
                    printf("Invalid command type: %s\n", type);
                }
                exit(0);
            } else if (pid < 0) {
                perror("Fork failed");
            }

            wait(NULL); // Wait for the child process to complete
        } else if (strcmp(command, "exit") == 0) {
            break; // Exit the shell
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}
