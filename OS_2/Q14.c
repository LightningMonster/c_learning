/* 10.2
Q14) Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following ‘list’ commands as
myshell$ list f dirname :- To print names of all the files in current
directory.
myshell$ list i dirname :- To print names and inodes of the files in the
current directory. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_INPUT 1024

// Function to tokenize the command input
void tokenizeInput(char *input, char *args[], int *argCount) {
    char *token = strtok(input, " \n");
    *argCount = 0;
    while (token != NULL) {
        args[(*argCount)++] = token;
        token = strtok(NULL, " \n");
    }
    args[*argCount] = NULL;
}

// Function to list filenames in a directory
void listFiles(char *dirname) {
    struct dirent *entry;
    DIR *dir = opendir(dirname);

    if (dir == NULL) {
        printf("Could not open directory: %s\n", dirname);
        return;
    }

    printf("Files in directory %s:\n", dirname);
    while ((entry = readdir(dir)) != NULL) {
        // Only display files (excluding '.' and '..')
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}

// Function to list filenames and inodes in a directory
void listFilesWithInodes(char *dirname) {
    struct dirent *entry;
    struct stat filestat;
    DIR *dir = opendir(dirname);

    if (dir == NULL) {
        printf("Could not open directory: %s\n", dirname);
        return;
    }

    printf("Files and Inodes in directory %s:\n", dirname);
    while ((entry = readdir(dir)) != NULL) {
        // Only display files and directories (excluding '.' and '..')
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirname, entry->d_name);
            if (stat(filepath, &filestat) == 0) {
                printf("File: %s | Inode: %ld\n", entry->d_name, filestat.st_ino);
            }
        }
    }
    closedir(dir);
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_INPUT];
    int argCount;

    while (1) {
        printf("myshell$ ");
        fgets(input, MAX_INPUT, stdin);

        // Tokenize the input
        tokenizeInput(input, args, &argCount);

        // Exit command to break the loop
        if (argCount > 0 && strcmp(args[0], "exit") == 0) {
            break;
        }

        // Handling 'list' command
        if (argCount > 2 && strcmp(args[0], "list") == 0) {
            if (strcmp(args[1], "f") == 0) {
                listFiles(args[2]);
            } else if (strcmp(args[1], "i") == 0) {
                listFilesWithInodes(args[2]);
            } else {
                printf("Invalid option. Use 'f' for filenames or 'i' for filenames and inodes.\n");
            }
        } else {
            printf("Command not recognized. Use 'list f dirname' or 'list i dirname'.\n");
        }
    }

    return 0;
}
