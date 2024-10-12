/* 25.2
Q30) Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret
the following commands.
myshell$ search f filename pattern :- To display first occurrence of
pattern in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to search for a pattern in a file
void search_in_file(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[1024];
    int line_number = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        if (strstr(line, pattern) != NULL) {
            printf("Pattern found in line %d: %s", line_number, line);
            fclose(file);
            return;
        }
    }
    
    printf("Pattern not found in the file.\n");
    fclose(file);
}

int main() {
    char command[1024];

    while (1) {
        // Display the prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read command from user
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;  // Exit on EOF
        }

        // Remove trailing newline character
        command[strcspn(command, "\n")] = 0;

        // Tokenize the command
        char *args[4];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Null-terminate the argument list

        // Check for the search command
        if (i >= 4 && strcmp(args[0], "search") == 0 && strcmp(args[1], "f") == 0) {
            // Fork a child process to execute the search
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                continue;
            } else if (pid == 0) {
                // In child process
                search_in_file(args[2], args[3]);
                exit(0);  // Terminate child process
            } else {
                // In parent process
                wait(NULL);  // Wait for child process to finish
            }
        } else if (strcmp(command, "exit") == 0) {
            // Exit the shell
            break;
        } else {
            printf("Unknown command or incorrect format. Please use 'search f filename pattern'.\n");
        }
    }

    return 0;
}
