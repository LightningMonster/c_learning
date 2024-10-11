/*  15.2
Q20) Write the program to simulate preemptive Shortest Job First (SJF) –
scheduling. The arrival time and first CPU-burst of different jobs should be
input to the system. Accept no. of Processes, arrival time and burst time. The
output should give Gantt chart, turnaround time and waiting time for each
process. Also find the average waiting time and turnaround time */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process_info {
    char pname[20];
    int at, bt, ct, bt1;
    struct process_info *next;
} NODE;

int n, time = 0;
NODE *first = NULL, *last = NULL;

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

        printf("Enter CPU burst time: ");
        scanf("%d", &p->bt);

        p->bt1 = p->bt;  // Remaining burst time
        p->next = NULL;

        if (first == NULL)
            first = p;
        else
            last->next = p;

        last = p;
    }
}

void print_output() {
    NODE *p;
    float avg_tat = 0, avg_wt = 0;

    printf("pname\tat\tbt\tct\ttat\twt\n");

    p = first;
    while (p != NULL) {
        int tat = p->ct - p->at;  // Turnaround Time = Completion Time - Arrival Time
        int wt = tat - p->bt;     // Waiting Time = Turnaround Time - Burst Time

        avg_tat += tat;
        avg_wt += wt;

        printf("%s\t%d\t%d\t%d\t%d\t%d\n", p->pname, p->at, p->bt, p->ct, tat, wt);

        p = p->next;
    }

    printf("Avg TAT = %f\t Avg WT = %f\n", avg_tat / n, avg_wt / n);
}

void print_input() {
    NODE *p = first;

    printf("pname\tat\tbt\n");
    while (p != NULL) {
        printf("%s\t%d\t%d\n", p->pname, p->at, p->bt1);
        p = p->next;
    }
}

NODE *get_sjf() {
    NODE *p = first, *min_p = NULL;
    int min_bt = 9999;

    while (p != NULL) {
        if (p->at <= time && p->bt1 > 0 && p->bt1 < min_bt) {
            min_bt = p->bt1;
            min_p = p;
        }
        p = p->next;
    }

    return min_p;
}

struct gantt_chart {
    int start;
    char pname[30];
    int end;
} s[100], s1[100];

int k = 0;

void sjfp() {
    int prev_time = 0, completed = 0;
    NODE *p;

    while (completed != n) {
        p = get_sjf();

        if (p == NULL) {  // No process ready to execute
            time++;
            s[k].start = prev_time;
            strcpy(s[k].pname, "*");  // Idle time
            s[k].end = time;

            prev_time = time;
            k++;
        } else {
            time++;  // Process executes for 1 unit of time
            s[k].start = prev_time;
            strcpy(s[k].pname, p->pname);
            s[k].end = time;

            prev_time = time;
            k++;

            p->bt1--;  // Decrement remaining burst time

            if (p->bt1 == 0) {
                p->ct = time;  // Completion time
                completed++;
            }
        }
    }
}

void print_gantt_chart() {
    int i, j, m;

    s1[0] = s[0];
    
    for (i = 1, j = 0; i < k; i++) {
        if (strcmp(s[i].pname, s1[j].pname) == 0)
            s1[j].end = s[i].end;
        else
            s1[++j] = s[i];
    }

    printf("%d", s1[0].start);
    for (i = 0; i <= j; i++) {
        m = (s1[i].end - s1[i].start);

        for (k = 0; k < m / 2; k++)
            printf("-");

        printf("%s", s1[i].pname);

        for (k = 0; k < (m + 1) / 2; k++)
            printf("-");

        printf("%d", s1[i].end);
    }
    printf("\n");
}

int main() {
    accept_info();
    sjfp();
    print_output();
    print_gantt_chart();

    return 0;
}
