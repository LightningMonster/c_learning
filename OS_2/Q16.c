/* 13.2, 19.2
Q16) Write the simulation program for Round Robin scheduling for given time
quantum. The arrival time and first CPU-burst of different jobs should be input
to the system. Accept no. of Processes, arrival time and burst time. The output
should give the Gantt chart, turnaround time and waiting time for each
process. Also display the average turnaround time and average waiting time.*/
#include <stdio.h>

struct Process {
    int pid;             // Process ID
    int arrival_time;     // Arrival time
    int burst_time;       // Burst time
    int remaining_time;   // Remaining time (used in Round Robin)
    int waiting_time;     // Waiting time
    int turnaround_time;  // Turnaround time
};

void calculateRoundRobin(struct Process proc[], int n, int time_quantum) {
    int time = 0, completed = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;
    
    // Gantt chart for displaying process execution order
    printf("Gantt Chart:\n");
    
    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (proc[i].remaining_time > 0 && proc[i].arrival_time <= time) {
                if (proc[i].remaining_time > time_quantum) {
                    time += time_quantum;
                    proc[i].remaining_time -= time_quantum;
                    printf("| P%d (%d) ", proc[i].pid, time);
                } else {
                    time += proc[i].remaining_time;
                    proc[i].turnaround_time = time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
                    proc[i].remaining_time = 0;
                    completed++;
                    printf("| P%d (%d) ", proc[i].pid, time);
                }
            }
        }
    }
    printf("|\n");

    // Display turnaround time and waiting time for each process
    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_waiting_time += proc[i].waiting_time;
        total_turnaround_time += proc[i].turnaround_time;
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].arrival_time, proc[i].burst_time, proc[i].waiting_time, proc[i].turnaround_time);
    }

    // Display average turnaround time and waiting time
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    int n, time_quantum;
    
    // Input number of processes and time quantum
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process proc[n];
    
    // Input process details
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter arrival time and burst time for Process P%d: ", i + 1);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time;
    }
    
    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);
    
    // Calculate Round Robin scheduling
    calculateRoundRobin(proc, n, time_quantum);
    
    return 0;
}
