/* 8.1, 12.1, 18.1
Q13) Write the simulation program for demand paging and show the page
scheduling and total number of page faults according the LRU page
replacement algorithm. Assume the memory of n frames.
Reference String : 8 5 7 8 5 7 2 3 7 3 5 9 4 6 2 */

#include <stdio.h>

int n, nf; // n = number of references, nf = number of frames
int ref_str[100]; // page reference string
int frames[50];   // frames
int counter[50];  // to keep track of the LRU order
int time = 0;     // global time to simulate the usage timing
int pg_faults = 0; // page fault counter

// Function to get user input
void getData() {
    printf("\nEnter the number of frames: ");
    scanf("%d", &nf);

    printf("Enter the length of page reference string: ");
    scanf("%d", &n);

    printf("Enter the page reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &ref_str[i]);
    }
}

// Function to initialize frames and counters
void initialize() {
    for (int i = 0; i < nf; i++) {
        frames[i] = -1;   // Initialize all frames to -1 (empty)
        counter[i] = 0;   // Initialize counters to 0
    }
}

// Function to check if a page is a hit
int isHit(int page) {
    for (int i = 0; i < nf; i++) {
        if (frames[i] == page) {
            counter[i] = ++time;  // Update the counter for the hit page
            return 1;
        }
    }
    return 0;
}

// Function to find the least recently used page's index
int getLRUIndex() {
    int min = counter[0], index = 0;
    for (int i = 1; i < nf; i++) {
        if (counter[i] < min) {
            min = counter[i];
            index = i;
        }
    }
    return index;
}

// Function to simulate the LRU page replacement algorithm
void LRU() {
    initialize();
    for (int i = 0; i < n; i++) {
        printf("\nReference %d -> ", ref_str[i]);
        
        if (!isHit(ref_str[i])) {
            int index = getLRUIndex(); // Find the least recently used page
            frames[index] = ref_str[i]; // Replace the page
            counter[index] = ++time;    // Update the usage time
            pg_faults++;  // Increment the page fault count

            // Display the current frames' status
            for (int j = 0; j < nf; j++) {
                if (frames[j] != -1) {
                    printf("%d ", frames[j]);
                } else {
                    printf("- "); // Empty frame
                }
            }
        } else {
            printf("No page fault");
        }
    }
    printf("\nTotal number of page faults: %d\n", pg_faults);
}

int main() {
    getData();
    LRU();
    return 0;
}

