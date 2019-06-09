#include <pthread.h>
#include <stdlib.h>

typedef struct __node_t
{
    int key;
    struct __note_t *next;
} node_t;

typedef struct __list_t
{
    node_t *head;
    pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L)
{
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key)
{
    pthread_mutex_lock(&L->lock);
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        pthread_mutex_unlock(&L->lock);
        return -1;
    }
    new->key = key;
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0;
}

int List_Lookup(list_t *L, int key)
{
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr)
    {
        if (curr->key == key)
        {
            pthread_mutex_unlock(&L->lock);
            return 0;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return -1;
}

typedef struct __queue_t
{
    node_t *head;
    node_t *tail;
    pthread_mutex_t headlock;
    pthread_mutex_t taillock;
} queue_t;

void Queue_Init(queue_t *q)
{
    node_t *tmp = malloc(sizeof(node_t));
    tmp->next = NULL;
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->headlock, NULL);
    pthread_mutex_init(&q->taillock, NULL);
}

void Queue_Enqueue(queue_t *q, int value)
{
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value = value;
    tmp->next = NULL;

    pthread_mutex_lock(&q->taillock);
    q->tail->next = tmp;
    q->tail = tmp;
    pthread_mutex_unlock(&q->taillock);
}

int Queue_Dequeue(queue_t *q, int *value)
{
    pthread_mutex_lock(&q->headlock);
    node_t *tmp = q->head;
    node_t *newhead = tmp->next;
    if (newhead == NULL)
    {
        pthread_mutex_unlock(&q->headlock);
        return -1;
    }
    *value = newhead->value;
    q->head = newhead;
    pthread_mutex_unlock(&q->headlock);
    free(tmp);
    return 0;
}

// Think about it: Why is the hash table concurrent???

#define BUCKETS 101

typedef struct __hash_t
{
    int scale;
    int counter;
    list_t[] lists;
    pthread_mutex_t lock;
} hash_t;

void Hash_Init(hash_t *H, int newscale)
{
    int len = BUCKETS * newscale;
    H->lists = malloc(len * sizeof(list_t));
    H->scale = 1;
    H->counter = 0;
    for (int i = 0; i < len; i++)
    {
        List_Init(&H->lists[i]);
    }
}

void Hash_Resize(hash_t *H, bool large)
{
    int newscale = H->scale;
    if (large)
        newscale = newscale * 2;
    else
        newscale = newscale / 2;
    hash_t *new_hash = malloc(size * sizeof(hash_t));
    Hash_Init(new_hash, newscale);
    pthread_mutex_lock(&H->lock);
    for (int i = 0; i < BUCKETS * H->scale; ++i)
    {
        node_t *node = H->lists[i]->head;
        while (node->next != NULL)
        {
            Hash_Insert(new_hash, node->key);
            node = node->next;
        }
    }
    pthread_mutex_t oldlock = H->lock;
    *H = *new_hash;
    pthread_mutex_unlock(&oldlock);
}

int Hash_Insert(hash_t *H, int key)
{
    if (H->counter / len >= 2)
        Hash_Resize(H, true);
    int bucket = key % BUCKETS;
    H->counter++;
    return List_Insert(&H->lists[bucket], key);
}

int Hash_Lookup(hash_t *H, int key)
{
    int bucket = key % BUCKETS;
    return List_Lookup(&H->lists[bucket], key);
}
