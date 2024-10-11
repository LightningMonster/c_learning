/*
 Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the Optimal page 
 replacement algorithm. Assume the memory of n frames.
Reference String : 7, 5, 4, 8, 5, 7, 2, 3, 1, 3, 5, 9, 4, 6, 2*/
#include <stdio.h>
#include <stdbool.h>

// Function to check if the page is already in the frame
bool isPageInFrames(int frames[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to find the optimal page to replace
int findOptimalPageIndex(int frames[], int n, int referenceString[], int refSize, int currentIndex) {
    int maxDistance = -1;
    int indexToReplace = -1;

    for (int i = 0; i < n; i++) {
        int page = frames[i];
        bool found = false;

        // Find when this page will be used in the future
        for (int j = currentIndex + 1; j < refSize; j++) {
            if (referenceString[j] == page) {
                if (j > maxDistance) {
                    maxDistance = j;
                    indexToReplace = i;
                }
                found = true;
                break;
            }
        }

        // If the page is not going to be used again, replace it
        if (!found) {
            return i;
        }
    }

    return (indexToReplace == -1) ? 0 : indexToReplace;
}

// Main function to simulate Optimal page replacement
void optimalPageReplacement(int referenceString[], int refSize, int framesCount) {
    int frames[framesCount];
    int pageFaults = 0;

    // Initialize the frames array to -1
    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < refSize; i++) {
        int currentPage = referenceString[i];
        printf("Processing page: %d\t", currentPage);

        if (!isPageInFrames(frames, framesCount, currentPage)) {
            pageFaults++;

            // If there's an empty frame, use it
            bool placed = false;
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    frames[j] = currentPage;
                    placed = true;
                    break;
                }
            }

            // If no empty frame, replace a page using the Optimal algorithm
            if (!placed) {
                int optimalIndex = findOptimalPageIndex(frames, framesCount, referenceString, refSize, i);
                frames[optimalIndex] = currentPage;
            }
        }

        // Display the frame status at each step
        printf("Step %d: ", i + 1);
        for (int k = 0; k < framesCount; k++) {
            if (frames[k] != -1) {
                printf("%d\t", frames[k]);
            } else {
                printf("-\t");
            }
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", pageFaults);
}

int main() {
    int referenceString[] = {7, 5, 4, 8, 5, 7, 2, 3, 1, 3, 5, 9, 4, 6, 2};
    int refSize = sizeof(referenceString) / sizeof(referenceString[0]);
    int framesCount;

    // User input for number of frames
    printf("Enter number of frames: ");
    scanf("%d", &framesCount);

    // Run the Optimal page replacement simulation
    optimalPageReplacement(referenceString, refSize, framesCount);

    return 0;
}

