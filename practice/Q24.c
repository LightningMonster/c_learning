/* 21.1
Q24) Write a C Program to create a child process using fork (), display parent and
child process id. Child process will display the message “I am Child Process”
and the parent process should display “I am Parent Process”. */
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    int pid = fork();

    if (pid>0)
    {
        printf("I am Parent Process\n");
        printf("ID: %d\n",getpid());
    }    
    else if(pid==0)
    {
        printf("I am Child Process\n");
        printf("ID: %d\n",getpid());
    }
    else
    {
        printf("failed to create child process");
    }
    return 0;
}