#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char command[1024];
    char *args[3];  // Use char * to store tokenized strings

    while (1) {
        printf("myshell$ ");
        fgets(command, sizeof(command), stdin);  // Input

        // Tokenizing
        args[0] = strtok(command, " ");
        args[1] = strtok(NULL, " ");
        args[2] = strtok(NULL, " \n");  // Handle newline at the end of input

        // Exit commandx
        if (args[0] && strcmp(args[0], "exit") == 0) {
            break;
        }

        // Count command
        if (args[0] && strcmp(args[0], "count") == 0 && args[1] && args[2]) {
            FILE *file = fopen(args[2], "r");

            if (!file) {
                printf("\nError while opening file\n");
                continue;  // Continue to next iteration instead of returning
            }

            int count = 0;
            char ch;
            char word[256];  // Corrected size from 265 to 256
            char option = args[1][0];  // Get the first character of the option

            if (option == 'c') {  // Correct string comparison using '=='
                while ((ch = fgetc(file)) != EOF) {
                    count++;
                }
                printf("Number of characters: %d\n", count);
            } 
            else if (option == 'w') {
                while (fscanf(file, "%s", word) != EOF) {
                    count++;
                }
                printf("Number of words: %d\n", count);
            } 
            else if (option == 'l') {
                while ((ch = fgetc(file)) != EOF) {
                    if (ch == '\n') {  // Correct character comparison
                        count++;
                    }
                }
                printf("Number of lines: %d\n", count);
            }

            fclose(file);  // Close the file after processing
        } else {
            printf("Invalid command or arguments\n");
        }
    }

    return 0;
}
