# CS 4410 附录

## 2.2 附录 锁

### Test and Set

```c
typedef struct __lock_t
{
    int flag;
} lock_t;

int TestAndSet(int *old_ptr, int new)
{
    // pretend this to be atomic
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}

void TSinit(lock_t *lock)
{
    lock->flag = 0;
}

void TSlock(lock_t *lock)
{
    while (TestAndSet(&lock->flag, 1) == 1)
        ;
}

void TSunlock(lock_t *lock)
{
    lock->flag = 0;
}
```

### Compare and Swap

```c
int CompareAndSwap(int *ptr, int expected, int new)
{
    // pretend this to be atomic
    int original = *ptr;
    if (original == expected)
        *ptr = new;
    return original;
}

void CSlock(lock_t *lock)
{
    while (CompareAndSwap(&lock->flag, 0, 1) == 1)
        ;
}
```

### Load Linked and Store Conditional

```c
int LoadLinked(int *ptr)
{
    // pretend this to be atomic
    return *ptr;
}

int StoreConditional(int *ptr, int value)
{
    // pretend this to be atomic
    if (NoUpdateToPtrSinceLoadLinkedToThisAddress(ptr))
    {
        *ptr = value;
        return 1;
    }
    else
        return 0;
}

void LLSCLock(lock_t *lock)
{
    while (LoadLinked(&lock->flag) || !StoreConditional(&lock->flag, 1))
        ;
}
```

### Fetch and Add

```c
int FetchAndAdd(int *ptr)
{
    // pretend this to be atomic
    int old = *ptr;
    *ptr = old + 1;
    return old;
}

typedef struct __FAlock_t
{
    int ticket;
    int turn;
} FAlock_t;

void FAlock_init(FAlock_t *lock)
{
    lock->ticket = 0;
    lock->turn = 0;
}

void FAlock(FAlock_t *lock)
{
    int myturn = FetchAndAdd(&lock->ticket);
    while (lock->turn != myturn)
        ;
}

void FAunlock(FAlock_t *lock)
{
    lock->turn = lock->turn + 1;
}
```

### Queue, Test and Set, Yield, Wakeup

```c
typedef struct queue_t
{
    // implementation of a queue
} queue_t;

typedef struct __Qlock_t
{
    int flag;
    int guard;
    queue_t *q;
} Qlock_t;

void Qlock_init(Qlock_t *m)
{
    m->flag = 0;
    m->guard = 0;
    queue_init(m->q);
}

void Qlock(Qlock_t *m)
{
    while (TestAndSet(&m->guard, 1) == 1)
        ;
    if (m->flag == 0)
    {
        m->flag = 1;
        m->guard = 0;
    }
    else
    {
        queue_add(m->q, gettid());
        m->guard = 0;
        park();
    }
}

void Qunlock(Qlock_t *m)
{
    while (TestAndSet(&m->guard, 1) == 1)
        ;
    if (queue_empty(m->q))
        m->flag = 0;
    else
        unpark(queue_remove(m->q));
    m->guard = 0;
}
```

### 基于Linux的Futex锁

```c
void mutex_lock(int *mutex){
    int v;
    if (atomic_bit_test_set(mutex, 31) == 0)
        return;
    atomic_increment(mutex);
    while (1){
        if (atomic_bit_test_set(mutex, 31) == 0){
            atomic_decrement(mutex);
            return;
        }
        v = *mutex;
        if (v >= 0)
            continue;
        futex_wait(mutex, v);
    }
}

void mutex_unlock (int *mutex){
    if (atomic_add_zero(mutex, 0x80000000))
        return;
    futex_wake(mutex);
}
```
