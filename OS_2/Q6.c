/* 3.2, 24.2
Q6) Write a programto implement the toy shell. It should display the command
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
#include <fcntl.h>

// Function to count characters, words, or lines in a file
void count_file(char type, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int characters = 0, words = 0, lines = 0;
    char ch, prev = '\0';

    while ((ch = fgetc(file)) != EOF) {
        characters++; // Count characters

        if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (prev != ' ' && prev != '\n' && prev != '\t') {
                words++; // Count words
            }
        }

        if (ch == '\n') {
            lines++; // Count lines
        }

        prev = ch;
    }

    // Count the last word if the file doesn't end with a space or newline
    if (prev != ' ' && prev != '\n' && prev != '\t') {
        words++;
    }

    fclose(file);

    if (type == 'c') {
        printf("Character count: %d\n", characters);
    } else if (type == 'w') {
        printf("Word count: %d\n", words);
    } else if (type == 'l') {
        printf("Line count: %d\n", lines);
    } else {
        printf("Invalid option for count\n");
    }
}

// Function to execute a command by creating a child process
void execute_command(char *args[]) {
    if (strcmp(args[0], "count") == 0 && args[1] != NULL && args[2] != NULL) {
        char type = args[1][0];
        count_file(type, args[2]);
    } else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process executes the command
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
            }
            exit(EXIT_FAILURE);
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
    char command[1024];
    char *args[64];

    while (1) {
        // Display the shell prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read the command from the user
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Error reading command");
            continue;
        }

        // Tokenize the command
        tokenize_command(command, args);

        // If no command entered, continue to the next loop
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



