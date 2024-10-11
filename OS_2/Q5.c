/* 3.1
Q5) Write the simulation program to implement demand paging and show the page
scheduling and total number of page faults according to the LRU (using
counter method) page replacement algorithm. Assume the memory of n
frames.
Reference String : 3,5,7,2,5,1,2,3,1,3,5,3,1,6,2*/
#include<stdio.h>

int n, nf;
int in[100];
int p[50];
int hit = 0;
int i, j, k;
int pgfaultcnt = 0;

void getData() {
    printf("\nEnter length of page reference sequence: ");
    scanf("%d", &n);
    printf("\nEnter the page reference sequence: ");
    for (i = 0; i < n; i++)
        scanf("%d", &in[i]);
    printf("\nEnter number of frames: ");
    scanf("%d", &nf);
}

void initialize() {
    pgfaultcnt = 0;
    for (i = 0; i < nf; i++)
        p[i] = 9999;
}

int isHit(int data) {
    hit = 0;
    for (j = 0; j < nf; j++) {
        if (p[j] == data) {
            hit = 1;
            break;
        }
    }
    return hit;
}

int getHitIndex(int data) {
    int hitind;
    for (k = 0; k < nf; k++) {
        if (p[k] == data) {
            hitind = k;
            break;
        }
    }
    return hitind;
}

void dispPages() {
    for (k = 0; k < nf; k++) {
        if (p[k] != 9999)
            printf(" %d", p[k]);
    }
}

void dispPgFaultCnt() {
    printf("\nTotal number of page faults: %d", pgfaultcnt);
}

void lru() {
    initialize();
    int least[50];
    for (i = 0; i < n; i++) {
        printf("\nFor %d: ", in[i]);

        if (isHit(in[i]) == 0) {
            for (j = 0; j < nf; j++) {
                int pg = p[j];
                int found = 0;
                for (k = i - 1; k >= 0; k--) {
                    if (pg == in[k]) {
                        least[j] = k;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    least[j] = -9999;
            }
            int min = 9999;
            int repindex;
            for (j = 0; j < nf; j++) {
                if (least[j] < min) {
                    min = least[j];
                    repindex = j;
                }
            }
            p[repindex] = in[i];
            pgfaultcnt++;

            dispPages();
        } else {
            printf("No page fault!");
        }
    }
    dispPgFaultCnt();
}

int main() {
    int choice;
    while (1) {
        printf("\nPage Replacement Algorithms\n1.Enter data\n2.LRU\n3.Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            getData();
            break;
        case 2:
            lru();
            break;
        default:
            return 0;
        }
    }
}
