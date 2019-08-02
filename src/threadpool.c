#include "threadpool.h"

typedef enum {
    immediate_shutdown  = 1,
    graceful_shutdown   = 2
} threadpool_shutdown_t;

static int threadpool_free(threadpool_t *pool);
static void *threadpool_worker(void *arg);

threadpool_t *threadpool_init(int thread_num)
{
    if (thread_num <= 0)
        goto err;
        //error_exit("threadpool_init");

    threadpool_t *pool;
    if ((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
        goto err;

    pool->thread_count = 0;
    pool->queue_size   = 0;
    pool->shutdown     = 0;
    pool->started      = 0;
    pool->threads      = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
    pool->head         = (threadpool_task_t *)malloc(sizeof(threadpool_task_t));

    if ((pool->threads == NULL) || (pool->head == NULL))
        goto err;

    pool->head->func = NULL;
    pool->head->arg  = NULL;
    pool->head->next = NULL;

    if (pthread_mutex_init(&(pool->lock), NULL) != 0)
        goto err;

    if (pthread_cond_init(&(pool->cond), NULL) != 0) {
        pthread_mutex_destroy(&(pool->lock));
        goto err;
    }

    for (int i = 0; i < thread_num; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, threadpool_worker, (void *)pool) != 0) {
            threadpool_destroy(pool, 0);
            return NULL;
        }
        // log_info

        pool->thread_count++;
        pool->started++;
    }

    return pool;

err:
    if (pool)
        threadpool_free(pool);

    return NULL;
}

int threadpool_add(threadpool_t *pool, void (*func)(void *), void *arg) {
    int rc, err = 0;
    if (pool == NULL || func == NULL) {
        // log_err
        return -1;
    }

    if (pthread_mutex_lock(&(pool->lock)) != 0) {
        // log_err
        return -1;
    }

    if (pool->shutdown) {
        err = threadpool_already_shutdown;
        goto out;
    }

    threadpool_task_t *task = (threadpool_task_t *)malloc(sizeof(threadpool_task_t));
    if (task == NULL) {
        // log_err
        goto out;
    }

    task->func  = func;
    task->arg   = arg;
    task->next  = pool->head->next; // ???
    pool->head->next = task;

    pool->queue_size++;

    rc = pthread_cond_signal(&(pool->cond));
    // check

out:
    if (pthread_mutex_unlock(&pool->lock) != 0) {
        // log_err
        return -1;
    }

    return err;
}

static int threadpool_free(threadpool_t *pool) {
    if (pool == NULL || pool->started > 0)
        return -1;

    if (pool->threads)
        free(pool->threads);

    threadpool_task_t *old;
    while (pool->head->next) {
        old = pool->head->next;
        pool->head->next = pool->head->next->next;
        free(old);
    }

    return 0;
}

int threadpool_destroy(threadpool_t *pool, int graceful) {
    int err = 0;

    if (pool == NULL) {
        // log_err
        return threadpool_invalid;
    }

    if (pthread_mutex_lock(&(pool->lock)) != 0)
        return threadpool_lock_fail;

    do {
        if (pool->shutdown) {
            err = threadpool_already_shutdown;
            break;
        }

        pool->shutdown = (graceful)? graceful_shutdown: immediate_shutdown;

        if (pthread_cond_broadcast(&(pool->cond)) != 0) {
            err = threadpool_cond_broadcast;
            break;
        }

        if (pthread_mutex_unlock(&(pool->lock)) != 0) {
            err = threadpool_lock_fail;
            break;
        }

        for (int i = 0; i < pool->thread_count; i++) {
            if (pthread_join(pool->threads[i], NULL) != 0)
                err = threadpool_thread_fail;
            // log_info
        }
    } while (0);    // ???

    if (!err) {
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->cond));
        threadpool_free(pool);
    }

    return err;
}

static void *threadpool_worker(void *arg)
{
    if (arg == NULL) {
        // log_err
        return NULL;
    }

    threadpool_t *pool = (threadpool_t *)arg;
    threadpool_task_t *task;

    while (1) {
        pthread_mutex_lock(&(pool->lock));

        while ((pool->queue_size == 0) && !(pool->shutdown))
            pthread_cond_wait(&(pool->cond), &(pool->lock));

        if (pool->shutdown == immediate_shutdown) {
            break;
        } else if ((pool->shutdown == graceful_shutdown) && pool->queue_size == 0) {
            break;
        }

        task = pool->head->next;
        if (task == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            continue;
        }

        pool->head->next = task->next;
        pool->queue_size--;

        pthread_mutex_unlock(&(pool->lock));

        (*(task->func))(task->arg);

        free(task);
    }

    pool->started--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);

    return NULL;
}
