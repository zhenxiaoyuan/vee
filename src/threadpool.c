#include "threadpool.h"

static void *vee_tp_worker(void *arg);
static int vee_tp_free(vee_tp_t *pool);

vee_tp_t *vee_tp_init(int thread_num)
{
    vee_tp_t* pool;
    if ((pool = (vee_tp_t *)malloc(sizeof(vee_tp_t))) == NULL)
        goto err;

    if (pthread_mutex_init(&(pool->mtx), NULL) != 0)
        goto err;

    if (pthread_cond_init(&(pool->cond), NULL) != 0) {
        pthread_mutex_destroy(&(pool->mtx));
        goto err;
    }

    if ((pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_num)) == NULL)
        goto err;

    if ((pool->head = (vee_task_t *)malloc(sizeof(vee_task_t))) == NULL)
        goto err;

    pool->thread_count = 0;
    pool->task_count = 0;
    pool->started = 0;
    pool->shutdown = 0;

    pool->head->func = NULL;
    pool->head->arg = NULL;
    pool->head->next = NULL;

    for (int i = 0; i < thread_num; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, vee_tp_worker, (void *)pool) != 0) {
            vee_tp_destroy(pool, 0);
            goto err;
        }

        pool->thread_count++;
        pool->started++;
        logger(INFO, "tid: %ld started.", pool->threads[i]);
    }

    logger(INFO, "%d threads already started.", pool->started++);

    return pool;

err:
    vee_tp_free(pool);

    return NULL;
}

void vee_tp_add(vee_tp_t *pool, void (*func)(void *), void *arg)
{
    int err = 0;

    // The difference between !pool and pool==NULL ?
    if (!pool || !func)
        err_exit("[vee_tp_add] pool or func is null");

    if (pthread_mutex_lock(&(pool->mtx)) != 0)
        err_exit("[vee_tp_add] pthread_mutex_lock error");

    if (pool->shutdown > 0) {
        err = vee_tp_already_shutdown;
        err_msg("[vee_tp_add] pool already shutdown");
        goto out;
    }

    //logger(INFO, "new task arrived.");

    vee_task_t *task;
    if ((task = (vee_task_t *)malloc(sizeof(vee_task_t))) == NULL) {
        err_msg("[vee_tp_add] malloc task error");
        goto out;
    }

    task->func = func;
    task->arg = arg;
    task->next = pool->head->next;

    pool->head->next = task;
    pool->task_count++;

    if (pthread_cond_signal(&(pool->cond)) != 0)
        err_msg("[vee_tp_add] cond signal error");

out:
    if (pthread_mutex_unlock(&(pool->mtx)) != 0)
        err_exit("[vee_tp_add] pthread_mutex_unlock error");
}

int vee_tp_destroy(vee_tp_t *pool, int graceful)
{
    int err = 0;

    if (!pool)
        return vee_tp_invalid;

    if (pthread_mutex_lock(&(pool->mtx)) != 0) {
        err = vee_tp_mtx_fail;
        goto err;
    }

    if (pool->shutdown > 0) {
        err = vee_tp_already_shutdown;
        goto err;
    }

    pool->shutdown = graceful ? graceful_shutdown : immediate_shutdown;

    if (pthread_cond_broadcast(&(pool->cond)) != 0) {
        err = vee_tp_cond_fail;
        goto err;
    }

    if (pthread_mutex_unlock(&(pool->mtx)) != 0) {
        err = vee_tp_mtx_fail;
        goto err;
    }

    for (int i = 0; i < pool->thread_count; i++) {
        if (pthread_join(pool->threads[i], NULL) != 0) {
            err = vee_tp_thread_fail;
        }
    }

err:
    pthread_mutex_destroy(&(pool->mtx));
    pthread_cond_destroy(&(pool->cond));
    vee_tp_free(pool);

    return err;
}

static void *vee_tp_worker(void *arg)
{
    if (arg == NULL)
        return NULL;

    vee_tp_t *pool = arg;
    vee_task_t *task;

    while(1) {
        if (pthread_mutex_lock(&(pool->mtx)) != 0)
            err_msg("[vee_tp_worker] lock mutex error");

        if ((pool->task_count == 0) && !(pool->shutdown)) {
            if (pthread_cond_wait(&(pool->cond), &(pool->mtx)) != 0)
                err_msg("[vee_tp_worker] wait condition error");
        }

        if (pool->shutdown == immediate_shutdown)
            break;
        else if ((pool->shutdown == graceful_shutdown) && (pool->task_count == 0))
            break;

        task = pool->head->next;
        if (task == NULL) {
            if (pthread_mutex_unlock(&(pool->mtx)) != 0)
                err_msg("[vee_tp_worker] unlock mutex error");
            continue;
        }

        pool->head->next = task->next;
        pool->task_count--;

        if (pthread_mutex_unlock(&(pool->mtx)) != 0)
            err_msg("[vee_tp_worker] unlock mutex error");

        logger(INFO, "starting thread worker, tid: %ld", pthread_self());
        task->func(task->arg);
        free(task);
    }

    pool->started--;
    if (pthread_mutex_unlock(&(pool->mtx)) != 0)
        err_msg("[vee_tp_worker] unlock mutex error");
    pthread_exit(NULL);

    return NULL;
}

static int vee_tp_free(vee_tp_t *pool)
{
    if (pool == NULL || pool->started > 0)
        return -1;

    if (pool->threads)
        free(pool->threads);

    vee_task_t *old;
    while (pool->head->next) {
        old = pool->head->next;
        pool->head->next = old->next;
        free(old);
    }

    return 0;
}
