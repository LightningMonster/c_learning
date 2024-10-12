/* 4.1
Q7) Write the simulation program for demand paging and show the page
scheduling and total number of page faults according the MFU page
replacement algorithm. Assume the memory of n frames.
Reference String : 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2 */
#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

int refString[MAX_PAGES], frames[MAX_FRAMES], freq[MAX_FRAMES];
int n, frameCount, pageFaults = 0;

// Function to check if a page is in memory (hit or miss)
int isPageInMemory(int page) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page) {
            freq[i]++; // Increase the frequency if it's a hit
            return 1;
        }
    }
    return 0;
}

// Function to find the index of the most frequently used page
int findMFUIndex() {
    int maxFreq = freq[0], mfuIndex = 0;
    for (int i = 1; i < frameCount; i++) {
        if (freq[i] > maxFreq) {
            maxFreq = freq[i];
            mfuIndex = i;
        }
    }
    return mfuIndex;
}

// Function to display the current frame contents
void displayFrames() {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
    printf("\n");
}

void mfuPageReplacement() {
    // Initialize frames and frequency array
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        freq[i] = 0;
    }

    printf("\nPage replacement process using MFU:\n");
    for (int i = 0; i < n; i++) {
        printf("Reference page: %d -> ", refString[i]);

        // Check if the page is already in memory (hit)
        if (!isPageInMemory(refString[i])) {
            // Page fault occurs
            pageFaults++;

            // If there is a free frame, insert the page there
            int inserted = 0;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    frames[j] = refString[i];
                    freq[j] = 1; // Set frequency to 1 for the new page
                    inserted = 1;
                    break;
                }
            }

            // If no free frame, replace the MFU page
            if (!inserted) {
                int mfuIndex = findMFUIndex();
                frames[mfuIndex] = refString[i];
                freq[mfuIndex] = 1; // Reset frequency for the new page
            }
            displayFrames(); // Display the updated frames
        } else {
            printf("Hit\n");
        }
    }

    printf("\nTotal number of page faults: %d\n", pageFaults);
}

int main() {
    // Reference string: 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2
    int reference[] = {8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2};
    n = sizeof(reference) / sizeof(reference[0]);

    // Copy the reference string to refString array
    for (int i = 0; i < n; i++) {
        refString[i] = reference[i];
    }

    // Number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);

    // Run MFU page replacement
    mfuPageReplacement();

    return 0;
}