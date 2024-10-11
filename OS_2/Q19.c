/* 15.1, 19.1
Q19) Write a C program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following ‘list’ commands as
myshell$ list f dirname :- To print names of all the files in current directory. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_CMD_LEN 100
#define MAX_ARGS 10

// Function to execute commands using child process
void execute_command(char *args[]) {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Execution failed");
        }
        exit(0);
    } else {
        // Parent process waits for the child to complete
        wait(NULL);
    }
}

// Function to list files in a directory
void list_files(const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    // Open directory
    if ((dir = opendir(dirname)) == NULL) {
        perror("Could not open directory");
        return;
    }

    // Read and print each file/directory in the directory
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// Function to handle the custom 'list' command
void handle_list_command(char *args[]) {
    if (args[1] != NULL && strcmp(args[1], "f") == 0 && args[2] != NULL) {
        list_files(args[2]);
    } else {
        printf("Invalid command. Usage: list f dirname\n");
    }
}

// Main function to implement the shell
int main() {
    char command[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *token;

    while (1) {
        // Display prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read user input
        if (fgets(command, MAX_CMD_LEN, stdin) == NULL) {
            perror("Failed to read command");
            exit(1);
        }

        // Remove newline character from the command
        command[strcspn(command, "\n")] = '\0';

        // Tokenize the command into arguments
        int i = 0;
        token = strtok(command, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Terminate the argument list with NULL

        // Check for 'list' command
        if (args[0] != NULL && strcmp(args[0], "list") == 0) {
            handle_list_command(args);
        } 
        // Exit the shell when 'exit' command is given
        else if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            break;
        } 
        // Execute other commands
        else {
            execute_command(args);
        }
    }

    return 0;
}
