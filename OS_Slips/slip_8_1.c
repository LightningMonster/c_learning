/* Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the LRU page 
 replacement algorithm. Assume the memory of n frames.
Reference String : 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2 */
#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_REF_STRING 100

// Function to find the least recently used page index
int findLRUIndex(int time[], int n) {
    int min = time[0], index = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < min) {
            min = time[i];
            index = i;
        }
    }
    return index;
}

// Main function to simulate LRU page replacement
void lruPageReplacement(int referenceString[], int refSize, int framesCount) {
    int frames[MAX_FRAMES], time[MAX_FRAMES];
    int pageFaults = 0, currentTime = 0;

    // Initialize frames to -1 (empty)
    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < refSize; i++) {
        int currentPage = referenceString[i];
        printf("Processing page: %d\t", currentPage);

        int found = 0;
        // Check if the page is already in one of the frames
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == currentPage) {
                found = 1;
                time[j] = currentTime++; // Update the time of access
                break;
            }
        }

        if (!found) {
            pageFaults++;

            // Check if there's an empty frame
            int placed = 0;
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    frames[j] = currentPage;
                    time[j] = currentTime++;
                    placed = 1;
                    break;
                }
            }

            // If no empty frame, replace the least recently used page
            if (!placed) {
                int lruIndex = findLRUIndex(time, framesCount);
                frames[lruIndex] = currentPage;
                time[lruIndex] = currentTime++;
            }
        }

        // Print the frames after each step
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
    int refSize = 15;  // Size of the reference string
    int framesCount;

    // Input number of frames
    printf("Enter number of frames: ");
    scanf("%d", &framesCount);

    // Run LRU page replacement simulation
    lruPageReplacement(referenceString, refSize, framesCount);

    return 0;
}

