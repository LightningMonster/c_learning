/*Write the simulation program for demand paging and show the page 
 scheduling and total number of page faults according the MFU page 
 replacement algorithm. Assume the memory of n frames.
Reference String : 8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2*/
#include <stdio.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

// Function to find the index of the most frequently used page
int find_mfu(int freq[], int n_frames) {
    int mfu_idx = 0;
    for (int i = 1; i < n_frames; i++) {
        if (freq[i] > freq[mfu_idx]) {
            mfu_idx = i;
        }
    }
    return mfu_idx;
}

// Function to check if the page is already in the frame
int is_in_frame(int frames[], int page, int n_frames) {
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == page) {
            return i; // Return index if found
        }
    }
    return -1; // Return -1 if not found
}

// MFU Page Replacement Algorithm
void mfu_page_replacement(int pages[], int ref_len, int n_frames) {
    int frames[MAX_FRAMES], freq[MAX_FRAMES] = {0};
    int page_faults = 0, next_free = 0;

    // Initialize frames with -1 to indicate they are empty
    for (int i = 0; i < n_frames; i++) {
        frames[i] = -1;
    }

    printf("\nPage Replacement Steps:\n");

    for (int i = 0; i < ref_len; i++) {
        int current_page = pages[i];
        int frame_idx = is_in_frame(frames, current_page, n_frames);

        printf("Processing page: %d  ", current_page); // Show current page being processed

        if (frame_idx == -1) {
            // Page fault occurs
            page_faults++;

            // If there's space, place the page in the next free frame
            if (next_free < n_frames) {
                frames[next_free] = current_page;
                freq[next_free] = 1; // Initialize frequency
                next_free++;
            } else {
                // If no free space, replace the MFU page
                int mfu_idx = find_mfu(freq, n_frames);
                frames[mfu_idx] = current_page;
                freq[mfu_idx] = 1; // Reset frequency
            }
        } else {
            // Page is already in the frame, increment its frequency
            freq[frame_idx]++;
        }

        // Print the current state of frames
        printf("Step %d: ", i + 1);
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] != -1) {
                printf("%d ", frames[j]);
            } else {
                printf("- ");
            }
        }
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
}

int main() {
    int pages[] = {8, 5, 7, 8, 5, 7, 2, 3, 7, 3, 5, 9, 4, 6, 2};
    int ref_len = sizeof(pages) / sizeof(pages[0]);
    int n_frames;

    // Input number of frames
    printf("Enter number of frames: ");
    scanf("%d", &n_frames);

    // Call the MFU page replacement function
    mfu_page_replacement(pages, ref_len, n_frames);

    return 0;
}

