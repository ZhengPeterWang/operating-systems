#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    printf("location of code: %p\n", main);
    printf("location of heap: %p\n", malloc(100e6));
    int x = 3;
    printf("location of stack: %p\n", &x);
    int *y = malloc(10 * sizeof(int));
    printf("%lu\n", sizeof(y));
    return x;
}