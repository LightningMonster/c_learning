/* 5.1, 7.1, 17.1, 23.2
Q9) Write the simulation program for demand paging and show the page
scheduling and total number of page faults according the optimal page
replacement algorithm. Assume the memory of n frames.
Reference String : 8 5 7 8 5 7 2 3 7 3 5 9 4 6 2 */
#include <stdio.h>
int n, nf;
int in[100];
int p[50];
int hit = 0;
int i, j, k;
int pgfaultcnt = 0;

void getData() {
    printf("\nEnter length of page reference sequence:");
    scanf("%d", &n);
    printf("\nEnter the page reference sequence:");
    for (i = 0; i < n; i++)
        scanf("%d", &in[i]);
    printf("\nEnter no of frames:");
    scanf("%d", &nf);
}

void initialize() {
    pgfaultcnt = 0;
    for (i = 0; i < nf; i++)
        p[i] = 9999;  // Initialize frames to a large number (9999)
}

int isHit(int data) {
    hit = 0;
    for (j = 0; j < nf; j++) {
        if (p[j] == data) {
            hit = 1;  // Page hit
            break;
        }
    }
    return hit;
}

void dispPages() {
    for (k = 0; k < nf; k++) {
        if (p[k] != 9999)  // Only display valid pages
            printf(" %d", p[k]);
    }
}

void dispPgFaultCnt() {
    printf("\nTotal no of page faults: %d", pgfaultcnt);
}

void optimal() {
    initialize();
    int near[50];
    for (i = 0; i < n; i++) {
        printf("\nFor %d :", in[i]);

        if (isHit(in[i]) == 0) {
            // Calculate the next occurrence of each page
            for (j = 0; j < nf; j++) {
                int pg = p[j];
                int found = 0;
                for (k = i; k < n; k++) {
                    if (pg == in[k]) {
                        near[j] = k;  // Store the next occurrence
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    near[j] = 9999;  // Set to a large value if not found
            }

            // Find the page with the farthest next occurrence
            int max = -1;
            int repindex = 0;
            for (j = 0; j < nf; j++) {
                if (near[j] > max) {
                    max = near[j];
                    repindex = j;  // Index of the page to replace
                }
            }

            p[repindex] = in[i];  // Replace the page
            pgfaultcnt++;  // Increment page fault count
            dispPages();  // Display current pages
        } else {
            printf("No page fault");
        }
    }
    dispPgFaultCnt();  // Display total page faults
}

int main() {
    int choice;
    while (1) {
        printf("\nPage Replacement Algorithms\n1. Enter data\n2. Optimal\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                getData();
                break;
            case 2:
                optimal();
                break;
            case 3:
                return 0;  // Exit the program
            default:
                return 0;  // Exit for invalid choice
        }
    }
}
