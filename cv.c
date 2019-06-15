// producer - consumer problem
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "common_threads.h"
#define MAX 100
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
pthread_mutex_t mutex;
pthread_cond_t empty, fill;
void put(int value)
{
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count ++;
}

int get()
{
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count --;
    return tmp;
}

void *producer(void *arg)
{
    int loops = (int)arg;
    for (int i = 0; i < loops; ++i)
    {
        Pthread_mutex_lock(&mutex);
        while (count == MAX)
            Pthread_cond_wait(&empty, &mutex);
        put(i);
        Pthread_cond_signal(&fill);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int loops = (int)arg;
    for (int i = 0; i < loops; ++i)
    {
        Pthread_mutex_lock(&mutex);
        while (count == 0)
            Pthread_cond_wait(&fill, &mutex);
        int tmp = get();
        Pthread_cond_signal(&empty);
        Pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
    return NULL;
}

int main(int argc, char * argv[]){
    int * num = malloc(sizeof(int));
    pthread_t p1, p2;
    printf("please input the num\n");
    scanf("%d", num);
    Pthread_create(&p1, NULL, producer, (void *)num);
    Pthread_create(&p2, NULL, consumer, (void *)num);
    Pthread_join(p2, NULL);
    Pthread_join(p1, NULL);
    printf("finished!");
    free(num);
    return 0;
}