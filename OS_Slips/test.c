#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_REF_STRING 100

// Function to find the least frequently used page's index
int findLFUIndex(int freq[], int n) {
    int minFreq = freq[0], minIndex = 0;
    for (int i = 1; i < n; i++) {
        if (freq[i] < minFreq) {
            minFreq = freq[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Main function to simulate LFU page replacement
void lfuPageReplacement(int referenceString[], int refSize, int framesCount) {
    int frames[MAX_FRAMES], freq[MAX_FRAMES], pageFaults = 0;

    // Initialize frames and frequencies
    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;  // Empty frames initially
        freq[i] = 0;     // Zero frequency initially
    }

    for (int i = 0; i < refSize; i++) {
        int currentPage = referenceString[i];
        printf("Processing page: %d\t", currentPage);
        
        // Check if page is already in frames
        int found = 0;
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == currentPage) {
                freq[j]++;  // Increase frequency if page found
                found = 1;
                break;
            }
        }

        // If page is not found, replace the least frequently used page
        if (!found) {
            pageFaults++;
            int replaceIndex = findLFUIndex(freq, framesCount);  // Find least frequently used page
            frames[replaceIndex] = currentPage;
            freq[replaceIndex] = 1;  // Reset frequency for the new page
        }

        // Print the frames at each step
        printf("Step %d: ", i + 1);
        for (int k = 0; k < framesCount; k++) {
            if (frames[k] != -1) {
                printf("%d  ", frames[k]);
            } else {
                printf("-\t");
            }
        }
        printf("\n");
    }

    printf("Total Page Faults: %d\n", pageFaults);
}

int main() {
    int referenceString[MAX_REF_STRING] = {3, 4, 5, 4, 3, 4, 7, 2, 4, 5, 6, 7, 2, 4, 6};
    int refSize = 15;  // Size of reference string
    int framesCount;

    // User input for number of frames
    printf("Enter number of frames: ");
    scanf("%d", &framesCount);

    // Run the LFU page replacement simulation
    lfuPageReplacement(referenceString, refSize, framesCount);

    return 0;
}
