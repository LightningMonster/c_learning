/* 22.1
Q26) Write a C program that demonstrates the use of nice() system call. After a
child Process is started using fork (), assign higher priority to the child using
nice () system call.*/
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        printf("Child Process, id=%d\n",getpid());
        printf("\nPriority: %d,id=%d\n",nice(-7),getpid());
    }
    else
    {
        printf("Parent Process, id=%d\n",getpid());
        nice(1);
        printf("\nPriority: %d,id=%d\n",nice(7),getpid());
    }
    return 0;
}