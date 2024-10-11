/* 16.1
Q21) Write a programto implement the toy shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following commands.
count c filename :- To print number of characters in the file.
count w filename :- To print number of words in the file.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

void count_chars(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    printf("Number of characters in %s: %ld\n", filename, size);
}

void count_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int word_count = 0;
    char word[MAX_CMD_LEN];

    while (fscanf(file, "%s", word) != EOF) {
        word_count++;
    }

    fclose(file);
    printf("Number of words in %s: %d\n", filename, word_count);
}

int main() {
    char command[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell$ ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Error reading command");
            continue;
        }

        // Remove newline character from command
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command
        int arg_count = 0;
        char *token = strtok(command, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Null-terminate the array of arguments

        if (arg_count > 0) {
            if (strcmp(args[0], "count") == 0) {
                if (arg_count != 3) {
                    printf("Usage: count <c/w> <filename>\n");
                    continue;
                }
                if (strcmp(args[1], "c") == 0) {
                    count_chars(args[2]);
                } else if (strcmp(args[1], "w") == 0) {
                    count_words(args[2]);
                } else {
                    printf("Invalid option. Use 'c' for characters or 'w' for words.\n");
                }
            } else if (strcmp(args[0], "exit") == 0) {
                printf("Exiting shell...\n");
                break;
            } else {
                printf("Unknown command: %s\n", args[0]);
            }
        }
    }

    return 0;
}
