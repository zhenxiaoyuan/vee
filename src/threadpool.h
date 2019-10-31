/*
 * 1. Producer feed a Task to Buffer
 *     1.1 Definition Task
 *     1.2 Add Task to Buffer
 * 2. Buffer store Task
 *     2.1 Definition Buffer use Queue
 *     2.2 Function handle new Task
 * 3. Consumer process Task from Buffer
 *     3.1 Definition ThreadPool
 *     3.2 Init ThreadPool
 *     3.3 Function take a Task to Thread
 */
#ifndef VEE_THREADPOOL_H
#define VEE_THREADPOOL_H

#include <pthread.h>
#include <stdlib.h>     /* use for malloc */

#include "error.h"

#define THREAD_NUM 8

typedef struct vee_task_s {
    void (*func)(void *);
    void *arg;
    struct vee_task_s *next;
} vee_task_t;

typedef struct vee_tp_s {
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    pthread_t *threads;
    int thread_count;
    vee_task_t *head;
    int task_count;
    int started;
    int shutdown;
} vee_tp_t;

typedef enum {
    vee_tp_invalid = -1,            // pool invalid
    vee_tp_mtx_fail = -2,           // mutex error
    vee_tp_cond_fail = -3,          // cond error
    vee_tp_thread_fail = -4,        // thread error
    vee_tp_already_shutdown = -5,
} vee_tp_err_t;

typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown = 2,
} vee_tp_shutdown_t;

// init threadpool
// create threads
// create task buffer
vee_tp_t *vee_tp_init(int thread_num);

// add task to buffer
void vee_tp_add(vee_tp_t *pool, void (*func)(void *), void *arg);

// distory thread
int vee_tp_destroy(vee_tp_t *pool, int graceful);

#endif  /* VEE_THREADPOOL_H */
