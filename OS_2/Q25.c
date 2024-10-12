/* 21.2
Q25)Write a C program to simulate Preemptive Priority scheduling. The arrival
time and first CPU-burst and priority for different n number of
processes should be input to the algorithm. Assume the fixed IO
waiting time (2 units). The next CPU-burst should be generated randomly. The
output should give Gantt chart, turnaround time and waiting time for each
process. Also find the average waiting time and turnaround time. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent each process
typedef struct process_info {
    char pname[20];
    int at, bt, ct, bt1, priority;
    struct process_info *next;
} NODE;

int n;
NODE *first, *last;
int time = 0;

void accept_info() {
    NODE *p;
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        p = (NODE *)malloc(sizeof(NODE));

        printf("Enter process name: ");
        scanf("%s", p->pname);

        printf("Enter arrival time: ");
        scanf("%d", &p->at);

        printf("Enter burst time: ");
        scanf("%d", &p->bt);

        printf("Enter priority (lower value means higher priority): ");
        scanf("%d", &p->priority);

        p->bt1 = p->bt;
        p->next = NULL;

        if (first == NULL) {
            first = p;
        } else {
            last->next = p;
        }
        last = p;
    }
}

void print_output() {
    NODE *p;
    float avg_tat = 0, avg_wt = 0;

    printf("\nPname\tAT\tBT\tCT\tTAT\tWT\n");

    p = first;
    while (p != NULL) {
        int tat = p->ct - p->at;  // Turnaround time
        int wt = tat - p->bt;     // Waiting time

        avg_tat += tat;
        avg_wt += wt;

        printf("%s\t%d\t%d\t%d\t%d\t%d\n", p->pname, p->at, p->bt, p->ct, tat, wt);
        p = p->next;
    }

    printf("\nAverage Turnaround Time = %f\n", avg_tat / n);
    printf("Average Waiting Time = %f\n", avg_wt / n);
}

NODE *get_highest_priority_process() {
    NODE *p = first, *selected = NULL;
    int min_priority = 9999;

    while (p != NULL) {
        if (p->at <= time && p->bt1 > 0 && p->priority < min_priority) {
            min_priority = p->priority;
            selected = p;
        }
        p = p->next;
    }

    return selected;
}

struct gantt_chart {
    int start;
    char pname[30];
    int end;
} gc[100], gc1[100];

int k = 0;

void priority_preemptive() {
    NODE *p;
    int completed = 0, prev_time = 0;

    while (completed != n) {
        p = get_highest_priority_process();

        if (p == NULL) {
            time++;  // If no process is ready, increase the time
        } else {
            if (k == 0 || strcmp(gc[k - 1].pname, p->pname) != 0) {
                // Add to Gantt chart if the process is different from the last one
                gc[k].start = time;
                strcpy(gc[k].pname, p->pname);
                k++;
            }

            time++;  // Increment time by 1 unit
            p->bt1--;  // Reduce the burst time of the selected process

            if (p->bt1 == 0) {
                p->ct = time;  // Completion time
                completed++;
            }

            gc[k - 1].end = time;  // Update the end time of the current process
        }
    }
}

void print_gantt_chart() {
    int i;

    printf("\nGantt Chart:\n");
    for (i = 0; i < k; i++) {
        printf("%d | %s (%d)", gc[i].start, gc[i].pname, gc[i].start);
    }
    printf(" %d\n", gc[k - 1].end);
}

int main() {
    accept_info();
    priority_preemptive();
    print_output();
    print_gantt_chart();
    
    return 0;
}
