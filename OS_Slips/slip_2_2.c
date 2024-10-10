/*Write a program to implement the shell. It should display the command 
 prompt “myshell$”. Tokenize the command line and execute the given 
 command by creating the child process. Additionally it should interpret the 
 following ‘list’ commands as 
 myshell$ list f dirname :- To print names of all the files in current 
 directory.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

// Function to list file names in a directory
void list_files(char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Only regular files
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}

// Function to list the number of entries (files and directories) in a directory
void list_number_of_entries(char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        count++;
    }
    printf("Total number of entries: %d\n", count);
    closedir(dir);
}

// Function to handle 'list' command
void handle_list_command(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: list f dirname or list n dirname\n");
        return;
    }

    char *option = args[1];
    char *dirname = args[2];

    if (strcmp(option, "f") == 0) {
        list_files(dirname);
    } else if (strcmp(option, "n") == 0) {
        list_number_of_entries(dirname);
    } else {
        printf("Invalid option. Use 'f' for file names or 'n' for number of entries.\n");
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

    // Custom 'list' command
    if (strcmp(args[0], "list") == 0) {
        handle_list_command(args);
    } else {
        // Execute external commands
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

