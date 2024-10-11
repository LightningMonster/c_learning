/*  2.1, 9.1, 10.1
Q3) Write the simulation program for demand paging and show the page
scheduling and total number of page faults according the FIFO page
replacement algorithm. Assume the memory of n frames.
Reference String : 3, 4, 5, 6, 3, 4, 7, 3, 4, 5, 6, 7, 2, 4, 6*/

#include<stdio.h>

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
    for(i = 0; i < n; i++)
        scanf("%d", &in[i]);
    printf("\nEnter no of frames:");
    scanf("%d", &nf);
}

void initialize() {
    pgfaultcnt = 0;
    for(i = 0; i < nf; i++)
        p[i] = 9999;
}

int isHit(int data) {
    hit = 0;
    for(j = 0; j < nf; j++) {
        if(p[j] == data) {
            hit = 1;
            break;
        }
    }
    return hit;
}

void dispPages() {
    for(k = 0; k < nf; k++) {
        if(p[k] != 9999)
            printf(" %d", p[k]);
    }
}

void dispPgFaultCnt() {
    printf("\nTotal no of page faults: %d", pgfaultcnt);
}

void fifo() {
    initialize();
    for(i = 0; i < n; i++) {
        printf("\nFor %d :", in[i]);

        if(isHit(in[i]) == 0) {
            for(k = 0; k < nf - 1; k++)
                p[k] = p[k + 1];

            p[k] = in[i];
            pgfaultcnt++;
            dispPages();
        } else {
            printf("No page fault");
        }
    }
    dispPgFaultCnt();
}

int main() {
    int choice;
    while(1) {
        printf("\nPage Replacement Algorithm\n1.Enter data\n2.FIFO\n3.Exit\nEnter your choice:");
        scanf("%d", &choice);
        switch(choice) {
        case 1:
            getData();
            break;
        case 2:
            fifo();
            break;
        default:
            return 0;
        }
    }
}
