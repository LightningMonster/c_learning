/*
 Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the MRU page 
 replacement algorithm. Assume the memory of n frames.
Reference String : 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2 */
#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_REF_STRING 100

// Function to check if the page is already in the frame
bool isPageInFrames(int frames[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to find the most recently used page index
int findMRUIndex(int recent[], int n) {
    int max = recent[0];
    int index = 0;

    for (int i = 1; i < n; i++) {
        if (recent[i] > max) {
            max = recent[i];
            index = i;
        }
    }

    return index;
}

// Main function to simulate MRU page replacement
void mruPageReplacement(int referenceString[], int refSize, int framesCount) {
    int frames[MAX_FRAMES];
    int recent[MAX_FRAMES];  // To keep track of the most recent usage of pages
    int pageFaults = 0;

    // Initialize the frames and recent array to -1
    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
        recent[i] = -1;
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
                    recent[j] = i;
                    placed = true;
                    break;
                }
            }

            // If no empty frame, replace the most recently used page
            if (!placed) {
                int mruIndex = findMRUIndex(recent, framesCount);
                frames[mruIndex] = currentPage;
                recent[mruIndex] = i;
            }
        } else {
            // Update the most recent usage
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == currentPage) {
                    recent[j] = i;
                    break;
                }
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
    int referenceString[MAX_REF_STRING] = {8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2};
    int refSize = 15;  // Size of reference string
    int framesCount;

    // User input for number of frames
    printf("Enter number of frames: ");
    scanf("%d", &framesCount);

    // Run the MRU page replacement simulation
    mruPageReplacement(referenceString, refSize, framesCount);

    return 0;
}

