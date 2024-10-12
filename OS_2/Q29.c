/* 25.1
Q29) Write a C program that accepts an integer array. 
Main function forks child process. 
Parent process sorts an integer array and 
passes the sorted array to child process through the command line arguments 
of execve() system call. The child process uses execve() system call to load 
new program that uses this sorted array for performing 
the binary search to search the particular item in the array.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to sort the array using bubble sort
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int arr[100], n, target, i;
    pid_t pid;

    // Input array size and elements
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    printf("Enter the array elements:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Input target element to search
    printf("Enter the element to search: ");
    scanf("%d", &target);

    // Sorting the array
    bubbleSort(arr, n);

    // Forking a new process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        char *args[100];  // Array of strings for execve arguments
        char executable[] = "./child_program";  // Name of the executable
        char *envp[] = {NULL};  // No environment variables

        // Adding the name of the executable as the first argument
        args[0] = executable;

        // Converting sorted array elements to strings and adding to args
        for (i = 0; i < n; i++) {
            args[i + 1] = malloc(10);
            sprintf(args[i + 1], "%d", arr[i]);
        }

        // Adding the target element as the last argument
        args[n + 1] = malloc(10);
        sprintf(args[n + 1], "%d", target);

        // Mark the end of arguments array with NULL
        args[n + 2] = NULL;

        // Execute the binary search program
        execve(args[0], args, envp);

        // If execve() fails
        perror("execve failed");
        exit(1);
    } else {
        // Parent process waits for the child to complete
        wait(NULL);
    }

    return 0;
}
