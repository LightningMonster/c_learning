/*Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the optimal page 
 replacement algorithm. Assume the memory of n frames.
Reference String : 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2*/

#include <stdio.h>

#define MAX 100

// Function to check if a page is in memory (frame)
int isInMemory(int frame[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (frame[i] == page) {
            return 1; // Page found in memory
        }
    }
    return 0; // Page not found in memory
}

// Function to find the page to be replaced using the optimal algorithm
int findOptimal(int frame[], int n, int refStr[], int refLen, int currentIndex) {
    int farthest = currentIndex, index = -1;

    for (int i = 0; i < n; i++) {
        int j;
        for (j = currentIndex; j < refLen; j++) {
            if (frame[i] == refStr[j]) {
                if (j > farthest) {
                    farthest = j;
                    index = i;
                }
                break;
            }
        }
        // If the page is never used in the future
        if (j == refLen) {
            return i;
        }
    }

    // If all pages are used in the future, replace the one that is farthest
    return (index == -1) ? 0 : index;
}

// Function to implement the optimal page replacement algorithm
void optimalPageReplacement(int refStr[], int refLen, int n) {
    int frame[n];
    int pageFaults = 0;

    // Initialize the frame array
    for (int i = 0; i < n; i++) {
        frame[i] = -1;
    }

    printf("Page Replacement Steps:\n");

    for (int i = 0; i < refLen; i++) {
        printf("Processing page: %d\t", refStr[i]);
        printf("Step %d: ", i + 1);

        // Check if the current page is already in memory
        if (!isInMemory(frame, n, refStr[i])) {
            // Find the page to be replaced if memory is full
            if (i < n) {
                // Directly place in frame if it's not full
                frame[i] = refStr[i];
            } else {
                int replaceIndex = findOptimal(frame, n, refStr, refLen, i + 1);
                frame[replaceIndex] = refStr[i];
            }

            pageFaults++;
        }

        // Display the current state of memory frames
        for (int j = 0; j < n; j++) {
            if (frame[j] != -1) {
                printf("%d\t", frame[j]);
            } else {
                printf("-\t");
            }
        }

        printf("\n");
    }

    // Total number of page faults
    printf("\nTotal Page Faults: %d\n", pageFaults);
}

int main() {
    int n; // Number of frames

    // Reference string
    int refStr[] = {8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2};
    int refLen = sizeof(refStr) / sizeof(refStr[0]);

    // Input: Number of frames
    printf("Enter number of frames: ");
    scanf("%d", &n);

    // Call the optimal page replacement algorithm
    optimalPageReplacement(refStr, refLen, n);

    return 0;
}

