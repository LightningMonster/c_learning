/* 23.1
Q27) Write a C program to illustrate the concept of orphan process. Parent process
creates a child and terminates before child has finished its task. So child
process becomes orphan process. (Use fork(), sleep(), getpid(), getppid()). */

#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>

int main(){
    int pid;
    pid = getpid;

    printf("Current process ID is: %d\n",getpid());
    printf("\n[Forking Child Process ... ] \n");

    pid=fork();
    if(pid<0)
    {
        printf("\nProcess can not be created ");
    }
    else
    {
        if(pid==0)
        {
            printf("\nChild is sleeping...\n");
            sleep(10);
            printf("\nOrphan childs parent ID: %d",getppid());
        }
        else
        {
            printf("\nParent process is completed");
        }
    }
    return 0;
}