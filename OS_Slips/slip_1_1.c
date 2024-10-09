/*
Q.1 Write the simulation program to implement demand paging and show the  page scheduling and total number of page faults according to the LFU page replacement algorithm. Assume the   memory of n frames.  
 Reference String : 3,4,5,4,3,4,7,2,4,5,6,7,2,4,6 
*/
#include <stdio.h>
#include <limits.h>

struct Page {
    int value;
    int frequency;
};

int findLFUPage(struct Page memory[], int n) {
    int minFreq = INT_MAX, lfuIndex = -1;

    for (int i = 0; i < n; i++) {
        if (memory[i].value != -1 && memory[i].frequency < minFreq) {
            minFreq = memory[i].frequency;
            lfuIndex = i;
        }
    }

    return lfuIndex;
}

int isPageInMemory(struct Page memory[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (memory[i].value == page) {
            return i; // Page found in memory, return its index
        }
    }
    return -1; // Page not found
}

void printMemory(struct Page memory[], int n) {
    for (int i = 0; i < n; i++) {
        if (memory[i].value != -1)
            printf("%d ", memory[i].value);
        else
            printf("- ");
    }
    printf("\n");
}

int main() {
    int n = 3; // Number of frames
    int referenceString[] = {3, 4, 5, 4, 3, 4, 7, 2, 4, 5, 6, 7, 2, 4, 6};
    int len = sizeof(referenceString) / sizeof(referenceString[0]);

    struct Page memory[n];
    for (int i = 0; i < n; i++) {
        memory[i].value = -1; // Initialize memory with empty frames
        memory[i].frequency = 0;
    }

    int pageFaults = 0;

    for (int i = 0; i < len; i++) {
        int currentPage = referenceString[i];
        printf("Reference: %d | Memory: ", currentPage);
        
        int pageIndex = isPageInMemory(memory, n, currentPage);
        
        if (pageIndex != -1) {
            // Page is already in memory, increase its frequency
            memory[pageIndex].frequency++;
        } else {
            // Page fault occurs
            pageFaults++;

            int emptySlot = isPageInMemory(memory, n, -1);
            if (emptySlot != -1) {
                // If there's an empty frame, place the page in it
                memory[emptySlot].value = currentPage;
                memory[emptySlot].frequency = 1;
            } else {
                // Find the Least Frequently Used (LFU) page and replace it
                int lfuIndex = findLFUPage(memory, n);
                memory[lfuIndex].value = currentPage;
                memory[lfuIndex].frequency = 1;
            }
        }

        printMemory(memory, n); // Print the memory state after each reference
    }

    printf("Total Page Faults: %d\n", pageFaults);
    return 0;
}
