#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXU);
    assert(fd > -1);
    int rc = fork();
    if (rc < 0)
    {
        printf("error");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("I am child: %d\n", (int)getpid());
        char buffer[20];
        sprintf(buffer, "hello world1\n");
        int rc = write(fd, buffer, strlen(buffer));
        assert(rc == (strlen(buffer)));
        fsync(fd);
        close(fd);
    }
    else
    {
        printf("I am parent: %d\n", (int)getpid());
        char buffer[20];
        sprintf(buffer, "hello world2\n");
        int rc = write(fd, buffer, strlen(buffer));
        assert(rc == (strlen(buffer)));
        fsync(fd);
        close(fd);
    }
    return 0;
}