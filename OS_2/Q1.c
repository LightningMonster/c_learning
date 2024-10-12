/* 1.1, 11.1
Q1) Write the simulation program to implement demand paging and show the
page scheduling and total number of page faults according to the LFU page replacement algorithm. Assume the memory of n frames.
Reference String : 3,4,5,4,3,4,7,2,4,5,6,7,2,4,6 */
#include <stdio.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_REF_STR_LEN 50

// Function to find the least frequently used frame
int findLFU(int frequency[], int frames[], int n) {
    int i, min = INT_MAX, pos = -1;
    
    for (i = 0; i < n; i++) {
        if (frames[i] != -1 && frequency[frames[i]] < min) {
            min = frequency[frames[i]];
            pos = i;
        }
    }
    
    return pos;
}

// Function to check if the page is already in memory (frames)
int isPageInFrames(int page, int frames[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (frames[i] == page)
            return i;  // Return index of the page if found
    }
    return -1;  // Page not found
}

// Function to print the frame contents
void printFrames(int frames[], int n) {
    int i;
    printf("[ ");
    for (i = 0; i < n; i++) {
        if (frames[i] != -1)
            printf("%d ", frames[i]);
        else
            printf("- ");
    }
    printf("]\n");
}

int main() {
    int frames[MAX_FRAMES];  // Page frames
    int frequency[MAX_REF_STR_LEN] = {0};  // Frequency of each page in the reference string
    int refStr[MAX_REF_STR_LEN];  // Reference string
    int n, refLen, i, pos, pageFaults = 0, filled = 0;

    // Input the number of frames
    printf("Enter the number of frames: ");
    scanf("%d", &n);

    // Input the reference string
    printf("Enter the length of the reference string: ");
    scanf("%d", &refLen);

    printf("Enter the reference string:\n");
    for (i = 0; i < refLen; i++) {
        scanf("%d", &refStr[i]);
    }

    // Initialize frames with -1 (indicating empty slots)
    for (i = 0; i < n; i++) {
        frames[i] = -1;
    }

    // Simulating LFU Page Replacement
    printf("\nPage Scheduling and Frame Status:\n");
    for (i = 0; i < refLen; i++) {
        int page = refStr[i];
        int index = isPageInFrames(page, frames, n);

        // If the page is not in frames, a page fault occurs
        if (index == -1) {
            // If there's an empty frame, use it
            if (filled < n) {
                frames[filled] = page;
                filled++;
            } else {
                // Find the least frequently used frame to replace
                pos = findLFU(frequency, frames, n);
                frames[pos] = page;
            }
            
            frequency[page]++;  // Update frequency of the current page
            pageFaults++;
            printf("Page %d caused a page fault: ", page);
        } else {
            // If the page is already in frames, just update its frequency
            frequency[page]++;
            printf("Page %d did not cause a page fault: ", page);
        }

        printFrames(frames, n);  // Print the current status of the frames
    }

    printf("\nTotal number of page faults: %d\n", pageFaults);

    return 0;
}
