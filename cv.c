#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "common_threads.h"
int buffer;
int count = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void put(int value)
{
    assert(count == 0);
    count = 1;
    buffer = value;
}

int get()
{
    assert(count == 1);
    count = 0;
    return buffer;
}

void *producer(void *arg)
{
    int loops = (int)arg;
    for (int i = 0; i < loops; ++i)
    {
        Pthread_mutex_lock(&mutex);
        if (count == 1)
            Pthread_cond_wait(&cond, &mutex);
        put(i);
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *arg)
{
    int loops = (int)arg;
    for (int i = 0; i < loops; ++i)
    {
        Pthread_mutex_lock(&mutex);
        if (count == 0)
            Pthread_cond_wait(&cond, &mutex);
        int tmp = get();
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
}