#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    sleep(20000);
    printf("hello world (pid: %d)\n", (int)getpid());
    int x = 1;
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        x = 2;
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        printf("child's x is %d", x);
    }
    else
    {
        x = 3;
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
        printf("parent's x is %d", x);
    }
    return 0;
}