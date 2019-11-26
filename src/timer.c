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
    pq = vee_pq_init(pq);
    vee_timer_update();
}

int vee_find_timer(void)
{
    long timer;
    vee_priority_queue_node_t *node;
    if(!vee_pq_is_empty(pq))
        node = vee_pq_min(pq);
    else
        return -1;

    vee_timer_update();
    timer = node->key - current_msec;
    return timer > 0 ? timer : 0;
}

void vee_expire_timers(void)
{
    vee_priority_queue_node_t *node;

    while (!vee_pq_is_empty(pq)) {
        node = vee_pq_min(pq);

        vee_timer_update();
        if ((node->key - current_msec) > 0)
            return;

        if (node->deleted == VEE_PQ_NODE_DELETED) {
            vee_pq_del_min(pq);
            continue;
        }

        /* Time out */
        node->handler((vee_http_request_t *)(node->data));

        vee_pq_del_min(pq);
    }
}

void vee_add_timer(vee_http_request_t *r, unsigned long timeout, handler_ptr handler)
{
    vee_priority_queue_node_t *node;
    /* TODO: Palloc later */
    if ((node = (vee_priority_queue_node_t *)malloc(sizeof(vee_priority_queue_node_t))) == NULL)
        err_exit("[vee_add_timer] malloc vee_priority_queue_node_t error");

    vee_timer_update();
    node->key = current_msec + timeout;
    node->data = (void *)r;
    node->deleted = VEE_PQ_NODE_NOT_DELETED;
    node->handler = handler;

    vee_pq_insert(pq, node);

    r->timer = (void *)node;
}

void vee_timer_del(vee_http_request_t *r)
{
    vee_timer_update();
    vee_priority_queue_node_t *node = (vee_priority_queue_node_t *)(r->timer);

    node->deleted = VEE_PQ_NODE_DELETED;
}
