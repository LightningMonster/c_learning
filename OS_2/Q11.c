/* 6.1
Q11) Write the simulation program for demand paging and show the page
scheduling and total number of page faults according the MRU page
replacement algorithm. Assume the memory of n frames.
Reference String : 8 5 7 8 5 7 2 3 7 3 5 9 4 6 2 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 100
#define MAX_REF_STR 100

int n, nf; // n = length of reference string, nf = number of frames
int ref_str[MAX_REF_STR]; // Reference string
int frames[MAX_FRAMES]; // Frame storage
int page_faults = 0; // Page fault counter

void get_data() {
    printf("Enter the length of the page reference sequence: ");
    scanf("%d", &n);
    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &ref_str[i]);
    }
    printf("Enter the number of frames: ");
    scanf("%d", &nf);
}

void initialize_frames() {
    for (int i = 0; i < nf; i++) {
        frames[i] = -1; // Initialize frames to -1 (empty)
    }
}

void display_frames() {
    printf("Frames: ");
    for (int i = 0; i < nf; i++) {
        if (frames[i] != -1) {
            printf("%d ", frames[i]);
        } else {
            printf("- "); // Indicate empty frame
        }
    }
    printf("\n");
}

int find_page(int page) {
    for (int i = 0; i < nf; i++) {
        if (frames[i] == page) {
            return i; // Return the index if page is found
        }
    }
    return -1; // Page not found
}

int find_most_recently_used(int current_index) {
    // Assume the first frame to replace initially
    int mru_index = 0;
    for (int i = 1; i < nf; i++) {
        if (frames[i] != -1) { // Check if frame is occupied
            // If the current frame is more recently used than the stored mru_index
            if (frames[mru_index] == ref_str[current_index]) {
                mru_index = i;
            }
        }
    }
    return mru_index;
}

void mru() {
    initialize_frames();
    for (int i = 0; i < n; i++) {
        printf("Referencing page: %d  ", ref_str[i]);
        int page_index = find_page(ref_str[i]);
        if (page_index == -1) {
            // Page fault occurs
            page_faults++;
            printf("Page fault! ");
            // Check for empty frames
            int empty_index = -1;
            for (int j = 0; j < nf; j++) {
                if (frames[j] == -1) {
                    empty_index = j;
                    break;
                }
            }

            if (empty_index != -1) {
                // Use empty frame
                frames[empty_index] = ref_str[i];
            } else {
                // No empty frame, replace the most recently used page
                int mru_index = find_most_recently_used(i);
                frames[mru_index] = ref_str[i];
            }
        } else {
            printf("No page fault.  ");
        }
        display_frames(); // Display current frame status
    }
}

int main() {
    get_data();
    mru(); // Call the MRU simulation
    printf("Total number of page faults: %d\n", page_faults);
    return 0;
}
