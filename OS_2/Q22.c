/* 16.2, 22.2
Q22) Write the program to simulate Non preemptive priority scheduling. The
arrival time and first CPU-burst of different jobs should be input to the
system. Accept no. of Processes, arrival time and burst time. The output
should give Gantt chart, turnaround time and waiting time for each process.
Also find the average waiting time and turnaround time.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process_info {
    char pname[20];
    int at;   // Arrival time
    int bt;   // Burst time
    int priority; // Priority
    int ct;   // Completion time
    int tat;  // Turnaround time
    int wt;   // Waiting time
} Process;

int n; // Number of processes
Process *processes;

void accept_info() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    processes = (Process *)malloc(n * sizeof(Process));

    for (int i = 0; i < n; i++) {
        printf("Enter process name: ");
        scanf("%s", processes[i].pname);
        printf("Enter arrival time: ");
        scanf("%d", &processes[i].at);
        printf("Enter burst time: ");
        scanf("%d", &processes[i].bt);
        printf("Enter priority (lower value means higher priority): ");
        scanf("%d", &processes[i].priority);
    }
}

void calculate_times() {
    int completed = 0, time = 0;
    int min_priority_index;

    while (completed < n) {
        min_priority_index = -1;

        // Find the process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= time && (min_priority_index == -1 || processes[i].priority < processes[min_priority_index].priority) && processes[i].bt > 0) {
                min_priority_index = i;
            }
        }

        if (min_priority_index != -1) {
            // Process found, update its completion time
            time += processes[min_priority_index].bt;
            processes[min_priority_index].ct = time;
            processes[min_priority_index].tat = processes[min_priority_index].ct - processes[min_priority_index].at;
            processes[min_priority_index].wt = processes[min_priority_index].tat - processes[min_priority_index].bt;
            processes[min_priority_index].bt = 0; // Mark process as completed
            completed++;
        } else {
            // No process is ready, increment time
            time++;
        }
    }
}

void print_output() {
    float avg_tat = 0, avg_wt = 0;

    printf("\npname\tat\tbt\tpriority\tct\ttat\twt\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t\t%d\t%d\t%d\n", processes[i].pname,
               processes[i].at, processes[i].bt == 0 ? processes[i].ct - processes[i].at : 0,
               processes[i].priority, processes[i].ct, processes[i].tat, processes[i].wt);
        avg_tat += processes[i].tat;
        avg_wt += processes[i].wt;
    }

    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Waiting Time: %.2f\n", avg_wt / n);
}

void print_gantt_chart() {
    int time = 0;
    printf("\nGantt Chart: \n");
    for (int i = 0; i < n; i++) {
        printf("| %s ", processes[i].pname);
        time = processes[i].ct;
    }
    printf("|");
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("----");
    }
    printf("%d\n", time);
}

int main() {
    accept_info();
    calculate_times();
    print_output();
    print_gantt_chart();
    
    free(processes); // Free allocated memory
    return 0;
}
