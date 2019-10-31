#### Threads Versus Processes
##### Advantages
1. Sharing data between threads is easy. （共享数据简单）
2. Thread creation is faster than process creation. （创建速度快）

##### Disadvantages
1. When programming with threads, we need to ensure that the functions we call are thread-safe or are called in a thread-safe manner. （需考虑线程安全）
2. A bug in one thread (e.g., modifying memory via an incorrect pointer) can damage all of the threads in the process, since they share the same address space and other attributes. （查bug难度大）
3. Each thread is competing for use of the finite virtual address space of the host process.（虚拟地址空间受限）


#### Mutex (Mutual exclusion)

1. Statically Allocated Mutexes  
```
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
```
2. Dynamically initializing
```
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
    //Returns 0 on success, or apositive error number on error

int pthread_mutex_destroy(pthread_mutex_t *mutex);
    //Returns 0 on success, or a positive error number on error
```
*It is not necessary to call pthread_mutex_destroy() on a mutex that was statically initialized using PTHREAD_MUTEX_INITIALIZER.*

3. Locked and Unlocked
```
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
    //Both return 0 on success, or a positive errornumber on error
```
> **没锁则锁，已锁则等**  
Mutex - Unlock  
Thread:A - Lock - Success  
Mutex - Lock  
Thread:B - Lock - Block  
Thread:A - Unlock - Success  
Mutex - Unlock  
Thread:B - Lock - Success  
Mutex - Lock  

> **连环锁变死锁**  
Mutex - Unlock  
Thread:A - Lock - Success  
Mutex - Lock  
Thread:A - Lock - DeadLock  

> **没锁别瞎解**  
Mutex - Unlock  
Thread:A - Unlock - Error  
> **不解别人的锁**  
Mutex - Unlock  
Thread:A - Lock - Success  
Mutex - Lock  
Thread:B - Unlock - Error  

> **抢锁看脸，各有输赢**  
Mutex - Unlock  
Thread:A - Lock - Success  
Mutex - Lock  
Thread:B - Lock - Block  
Thread:C - Lock - Block  
Thread:D - Lock - Block  
Thread:A - Unlock - Success  
Mutex - Unlock  
Thread:B - Lock - Maybe  
Thread:C - Lock - Maybe  
Thread:D - Lock - Maybe  
Mutex - Lock - B/C/D (indeterminate)  

4. Avoid Mutex Deadlocks
    - Mutex hierarchy.
    - Try, and then back off.

#### Condition Variables

1. Statically Allocated Condition Variables  
```
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```
2. Dynamically Allocated Condition Variables
```
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
    //Returns 0 on success, or a positive error number on error
int pthread_cond_destroy(pthread_cond_t *cond);
    //Returns 0 on success, or a positive error number on error
```

3. Signaling and Waiting on Condition Variables  
```
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
    //All return 0 on success, or a positive error number on error
```

pthread_cond_signal()
at least one of the blocked threads is woken up  
pthread_cond_broadcast()
all blocked threads are woken up

> **有锁有条件，解锁扔条件**  
Mutex - Unlock  
Thread:A - Lock - Success  
Mutex - Lock  
Thread:B - Lock - Block  
Thread:B - Cond - Wait  
Thread:A - Unlock - Success  
Mutex - Unlock  
Thread:A - Cond - Signal  
Thread:B - Lock - Success  
Mutex - Lock  

#### 生产者/消费者 Producer/Consumer
