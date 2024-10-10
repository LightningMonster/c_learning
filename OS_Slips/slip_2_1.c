/*Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the FIFO page 
 replacement algorithm. Assume the memory of n frames.
 Reference String : 3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6 */
#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_REF_STRING 100

// Function to check if a page is in the frame
bool isPageInFrame(int frames[], int num_frames, int page) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

// Function to simulate FIFO page replacement algorithm
void simulateFIFO(int ref_string[], int ref_length, int num_frames) {
    int frames[MAX_FRAMES];   // Memory frames
    int front = 0;            // Points to the oldest page
    int page_faults = 0;      // Count of page faults

    // Initialize the frames with -1 (empty)
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    printf("Reference String\tFrames\n");

    for (int i = 0; i < ref_length; i++) {
        int current_page = ref_string[i];

        // Check if the current page is already in any frame
        if (!isPageInFrame(frames, num_frames, current_page)) {
            // If it's not, replace the oldest page (FIFO logic)
            frames[front] = current_page;
            front = (front + 1) % num_frames; // Move the pointer to the next oldest frame
            page_faults++;

            // Print the current state of frames
            printf("%d\t\t\t", current_page);
            for (int j = 0; j < num_frames; j++) {
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
    int ref_string[] = {3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6}; // Reference string
    int ref_length = sizeof(ref_string) / sizeof(ref_string[0]);

    // Simulate FIFO page replacement
    simulateFIFO(ref_string, ref_length, num_frames);

    return 0;
} 
