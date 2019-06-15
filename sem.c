#include <semaphore.h>
#define MAX 1
sem_t empty;
sem_t full;
sem_t mutex;
int buffer [MAX];
int fill = 0;
int use = 0;

void put(int value){
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get(){
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *arg){
    int loops = (int) arg;
    for (int i = 0; i < loops; ++i){
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *arg){
    int i, tmp = 0;
    while(tmp != -1){
        sem_wait(&full);
        sem_wait(&mutex);
        tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("%d\n", tmp);
    }
}

int main(int argc, char *argv[]){
    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);
}