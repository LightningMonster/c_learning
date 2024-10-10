/*Write the simulation program to implement demand paging and show the page 
 scheduling and total number of page faults according to the LRU (using 
 counter method) page replacement algorithm. Assume the memory of n 
 frames. 
 Reference String : 3,5,7,2,5,1,2,3,1,3,5,3,1,6,2*/
#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_REF_STRING 100

// Function to find the LRU page
int find_LRU(int time[], int n) {
    int i, minimum = time[0], pos = 0;

    for (i = 1; i < n; i++) {
        if (time[i] < minimum) {
            minimum = time[i];
            pos = i;
        }
    }

    return pos;
}

// Function to simulate LRU page replacement algorithm
void simulateLRU(int ref_string[], int ref_length, int num_frames) {
    int frames[MAX_FRAMES];    // Memory frames
    int time[MAX_FRAMES];      // Stores the time of reference for each frame
    int page_faults = 0;       // Count of page faults
    int counter = 0;           // To track the current time
    int i, j, pos;

    // Initialize the frames and time with -1 (empty)
    for (i = 0; i < num_frames; i++) {
        frames[i] = -1;
        time[i] = -1;
    }

    printf("Reference String\tFrames\n");

    for (i = 0; i < ref_length; i++) {
        int current_page = ref_string[i];
        int found = 0;

        // Check if the current page is already in any frame
        for (j = 0; j < num_frames; j++) {
            if (frames[j] == current_page) {
                found = 1;
                time[j] = ++counter;  // Update the reference time
                break;
            }
        }

        if (!found) {
            // If the page is not found, it means a page fault has occurred
            pos = find_LRU(time, num_frames); // Find the LRU page
            frames[pos] = current_page;       // Replace the LRU page with the current page
            time[pos] = ++counter;            // Update the reference time
            page_faults++;

            // Print the current state of frames
            printf("%d\t\t\t", current_page);
            for (j = 0; j < num_frames; j++) {
                if (frames[j] != -1) {
                    printf("%d ", frames[j]);
                } else {
                    printf("- ");
                }
            }
            printf("\n");
        } else {
            // Print the current state if no page fault occurs
            printf("%d\t\t\t(No page fault)\n", current_page);
        }
    }

    printf("\nTotal number of page faults: %d\n", page_faults);
}

int main() {
    int num_frames = 3;  // Number of frames in memory
    int ref_string[] = {3, 5, 7, 2, 5, 1, 2, 3, 1, 3, 5, 3, 1, 6, 2}; // Reference string
    int ref_length = sizeof(ref_string) / sizeof(ref_string[0]);

    // Simulate LRU page replacement
    simulateLRU(ref_string, ref_length, num_frames);

    return 0;
}

