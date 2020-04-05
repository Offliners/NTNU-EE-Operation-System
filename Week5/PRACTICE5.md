# Practice 5
### Producer process using POSIX shared-memory API
POSIX shared-memory is organized using memory-mapped files(記憶體映射檔案)

```C
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>

int main()
{
        const int SIZE = 4096;
        const char *name = "OS";
        const char *message0 = "Hello";
        const char *message1 = "World!";
        int shm_fd;
        void *ptr;

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        sprintf(ptr, "%s", message0);
        ptr += strlen(message0);
        sprintf(ptr, "%s", message1);
        ptr += strlen(message1);

        return 0;
}
```
[code](producer.c)

### Consumer Process
```C
#include<fcntl.h>
#include<sys/mman.h>
#include<stdio.h>

int main()
{
        const char *name = "OS";
        const int SIZE = 4096;

        int shm_fd;
        void *ptr;
        int i;

        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("%s", (char*)ptr);
        shm_unlink(name);

        return 0;
}
```
[code](consumer.c)

### Quiz5
Try to using shared memory
```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var;

int main(void)
{
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *glob_var = 1;
    if (fork() == 0) {
        *glob_var = 5;
        exit(EXIT_SUCCESS);
    } 
    else {
        wait(NULL);
        printf("%d\n", *glob_var);
        munmap(glob_var, sizeof *glob_var);
    }

    return 0;
}
```
[code](quiz5.c)


### Quiz 6
Using shared memory
```C
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<wait.h>

#define SIZE 5

int main()
{
        int i;
        pid_t pid;
        int *nums;

        nums = mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        for(i = 0; i < SIZE; i++)
                nums[i] = i;

        pid = fork();
        if(pid == 0)
                for(i = 0; i < SIZE; i++)
                {
                        nums[i] *= -i;
                        printf("CHILD %d\n", nums[i]);
                }
        else if(pid > 0)
        {
                wait(NULL);
                for(i = 0; i < SIZE; i++)
                        printf("PARENT : %d\n", nums[i]);
        }

        return 0;
}

```
[code](quiz6.c)

### Quiz 7 
```C
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<wait.h>

#define SIZE 5

int main()
{
        int i;
        pid_t pid;
        int shm_fd;
        int *ptr;
        const char *name = "sharedMem";
        const int SHM_SIZE = SIZE * sizeof(int);

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SHM_SIZE);
        ptr = (int*)mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

        for(i = 0; i < SIZE; i++)
                ptr[i] = i;

        pid = fork();
        if(pid == 0)
                for(i = 0; i < SIZE; i++)
                {
                        ptr[i] *= -i;
                        printf("CHILD : %d\n", ptr[i]);
                }
        else if(pid > 0)
        {
                wait(NULL);
                for(i = 0; i < SIZE; i++)
                        printf("PARENT : %d\n", ptr[i]);
        }

        return 0;
}

```
[code](quiz7.c)