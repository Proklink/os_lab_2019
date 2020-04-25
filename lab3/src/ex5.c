#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
     //const char* argv = {"2","10",NULL}; 
     

    pid = fork();
    if(pid < 0)
    {
        printf("fork failed");
        return 1;
    }
    else if(pid == 0)
    {
        execl("sequential_min_max","sequential_min_max","2","10",NULL);
    }

    wait(NULL);
    printf("The end of parent process\n");

    return 0;
}