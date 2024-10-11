/*  2.2, 11.2, 12.2
Q4) Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following ‘list’ commands as
myshell$ list f dirname :- To print names of all the files in current
directory.
myshell$ list n dirname :- To print the number of all entries in the current
directory*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

// Function to print all the file names in the given directory
void list_files(char *dirname) {
    struct dirent *de; // Pointer for directory entry
    DIR *dr = opendir(dirname);
    if (dr == NULL) {
        printf("Could not open directory\n");
        return;
    }

    printf("Files in directory '%s':\n", dirname);
    while ((de = readdir(dr)) != NULL) {
        if (de->d_type == DT_REG) { // Check if the entry is a regular file
            printf("%s\n", de->d_name);
        }
    }

    closedir(dr);
}

// Function to print the number of all entries (files and directories) in the given directory
void list_entries(char *dirname) {
    struct dirent *de;
    int count = 0;
    DIR *dr = opendir(dirname);
    if (dr == NULL) {
        printf("Could not open directory\n");
        return;
    }

    while ((de = readdir(dr)) != NULL) {
        count++;
    }
    printf("Total number of entries in directory '%s': %d\n", dirname, count);

    closedir(dr);
}

// Function to handle the 'list' command
void execute_list_command(char *args[]) {
    if (strcmp(args[1], "f") == 0 && args[2] != NULL) {
        list_files(args[2]); // List files in the directory
    } else if (strcmp(args[1], "n") == 0 && args[2] != NULL) {
        list_entries(args[2]); // List number of entries in the directory
    } else {
        printf("Invalid list command format\n");
        printf("Usage: list f dirname or list n dirname\n");
    }
}

// Function to execute the given command
void execute_command(char *args[]) {
    if (strcmp(args[0], "list") == 0) {
        execute_list_command(args); // Handle custom 'list' command
    } else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
            }
            exit(EXIT_FAILURE);
        } else {
            // Parent process waits for the child to finish
            wait(NULL);
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

        // If no command entered, continue to next loop
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
