#include <stdlib.h>     /* used for malloc */

#include "priority_queue.h"
#include "error.h"

vee_priority_queue_t *vee_pq_init(vee_priority_queue_t *pq)
{
    if ((pq = (vee_priority_queue_t *)malloc(sizeof(vee_priority_queue_t))) == NULL)
        err_exit("[vee_pq_init] malloc priority queue error");

    if ((pq->nodes = (vee_priority_queue_node_t **)malloc
                (sizeof(vee_priority_queue_node_t *) * VEE_MAX_PQ_SIZE)) == NULL)
        err_exit("[vee_pq_init] malloc priority queue nodes error");

    pq->size = 0;

    /* Initialize the first node of priority queue. */
    vee_priority_queue_node_t *node;
    if ((node = (vee_priority_queue_node_t *)malloc(sizeof(vee_priority_queue_node_t))) == NULL)
        err_exit("[vee_pq_init] malloc priority queue first node error");

    node->key = VEE_PQ_NODE_MIN_DATA;       /* Dummy piece of information for `percolate up` */
    node->data = (void *)NULL;
    node->deleted = VEE_PQ_NODE_NOT_DELETED;

    pq->nodes[0] = node;

    return pq;
}

void vee_pq_insert(vee_priority_queue_t *pq, vee_priority_queue_node_t *node)
{
    if (vee_pq_not_inited(pq))
        err_exit("[vee_pq_insert] priority queue has not been initialized");

    if (vee_pq_is_full(pq))
        // TODO: Dynamic enlarge `nodes` memory space. What to do if space left?
        err_msg("[vee_pq_insert] priority queue is full");

    if (vee_pq_node_is_null(node))
        err_exit("[vee_pq_insert] priority queue node is null");

    /* Percolate up */
    int i;
    for (i = ++pq->size; pq->nodes[i/2]->key > node->key; i /= 2) {
        pq->nodes[i] = pq->nodes[i/2];
    }

    pq->nodes[i] = node;
}

void vee_pq_del_min(vee_priority_queue_t *pq)
{
    int i, child;
    vee_priority_queue_node_t *min, *last;

    min = vee_pq_min(pq);   /* min = pq->nodes[1] */
    last = pq->nodes[pq->size--];

    /* Percolate down */
    for (i = 1; i <= pq->size; i = child) {
        /* Find smaller chile */
        child = i * 2;
        if (child != pq->size && pq->nodes[child]->key > pq->nodes[child+1]->key)
            child++;

        /* Percolate one level */
        if (last->key > pq->nodes[child]->key)
            pq->nodes[i] = pq->nodes[child];
        else
            break;
    }

    pq->nodes[i] = last;

    /* TODO: Close connection while node deleted */

    free(min);

    return;
}

vee_priority_queue_node_t *vee_pq_min(vee_priority_queue_t *pq)
{
    vee_priority_queue_node_t *min;

    if (vee_pq_not_inited(pq))
        err_exit("[vee_pq_min] priority queue has not been initialized");

    min = pq->nodes[1];

    return min;
}

void vee_pq_node_set_deleted(vee_priority_queue_node_t *node)
{
    if (vee_pq_node_is_null(node))
        err_exit("[vee_pq_insert] priority queue node is null");

    if (VEE_PQ_NODE_DELETED != node->deleted)
        node->deleted = VEE_PQ_NODE_DELETED;

    /* TODO: Close connection while node deleted */
}

int vee_pq_is_empty(vee_priority_queue_t *pq)
{
    return !pq->size;
}

int vee_pq_is_full(vee_priority_queue_t *pq)
{
    return ((pq->size+1) == VEE_MAX_PQ_SIZE);
}

int vee_pq_not_inited(vee_priority_queue_t *pq)
{
    return (!pq || !pq->nodes);
}

int vee_pq_node_is_null(vee_priority_queue_node_t *node)
{
    return (NULL == node);
}
