#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
        pid_t child_pid = fork();

        if(child_pid > 0)
                sleep(50);
        else
                exit(0);

        return 0;
}
