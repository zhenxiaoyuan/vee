#include <sys/time.h>
#include <stdlib.h>

#include "timer.h"
#include "error.h"

static void vee_timer_update(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        err_exit("[vee_timer_update] gettimeofday error");

    current_msec = tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void vee_timer_init(void)
{
    vee_pq_init(pq);
    vee_timer_update();
}

int vee_find_timer(void)
{
    int timer;
    vee_priority_queue_node_t *node;
    node = vee_pq_min(pq);

    vee_timer_update();
    timer = node->key - current_msec;
    return timer > 0 ? timer : 0;
}

void vee_expire_timers(void)
{
    vee_priority_queue_node_t *node;
    // vee_http_request_t * r;

    while (!vee_pq_is_empty(pq)) {
        node = vee_pq_min(pq);

        vee_timer_update();
        if ((node->key - current_msec) > 0)
            return;

        /* Is node->deleted necessary if close fd while delete the minimum node? */
        if (node->deleted == VEE_PQ_NODE_DELETED) {
            vee_pq_del_min(pq);
            continue;
        }

        vee_pq_del_min(pq);
    }
}

void vee_add_timer(vee_conn_t *conn, unsigned long timeout)
{
    vee_priority_queue_node_t *node;
    /* TODO: Palloc later */
    if ((node = (vee_priority_queue_node_t *)malloc(sizeof(vee_priority_queue_node_t))) == NULL)
        err_exit("[vee_add_timer] malloc vee_priority_queue_node_t error");

    vee_timer_update();
    node->key = current_msec + timeout;
    node->data = (void *)conn;
    node->deleted = VEE_PQ_NODE_NOT_DELETED;

    vee_pq_insert(pq, node);
}
