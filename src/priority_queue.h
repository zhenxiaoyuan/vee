#ifndef VEE_PRIORITY_QUEUE_H
#define VEE_PRIORITY_QUEUE_H

#define VEE_MAX_PQ_SIZE         65535
#define VEE_PQ_NODE_DELETED     1
#define VEE_PQ_NODE_MIN_DATA    -1

/*
 * Priority queue node structure.
 */
typedef struct vee_priority_queue_node_s {
    int             key;       /* Comparable key to determine priority TODO: change char to time */
    void            *data;      /* Data */
    unsigned short  deleted;    /* 1-Yes | 0-No */
} vee_priority_queue_node_t;

/* 
 * Priority queue (not using the first entry of `nodes`).
 * 
 * The parent of the node in position k is in position k/2 and,
 * conversely, the two children of the node in the position k
 * are in positions 2k and 2k + 1.
 * 
 * TODO: Dynamic enlarge `nodes` memory space.
 */
typedef struct vee_priority_queue_s {
    vee_priority_queue_node_t **nodes;      /* An Array `nodes` store all pointers of node */
    unsigned int                size;       /* The number of nodes in priority queue */
} vee_priority_queue_t;

/*
 * Initialize a priority queue.
 */
void vee_pq_init(vee_priority_queue_t *pq);

/*
 * Insert a node into priority queue.
 */
void vee_pq_insert(vee_priority_queue_t *pq, vee_priority_queue_node_t *node);

/*
 * Delete the minimum node.
 */
vee_priority_queue_node_t *vee_pq_del_min(vee_priority_queue_t *pq);

/*
 * Return the minimum node.
 */
vee_priority_queue_node_t *vee_pq_min(vee_priority_queue_t *pq);

/*
 * Set the deleted flag of `node`.
 */
void vee_pq_node_set_deleted(vee_priority_queue_node_t *node);

/*
 * Is the priority queue empty?
 */
int vee_pq_is_empty(vee_priority_queue_t *pq);

/*
 * Is the priority queue full?
 */
int vee_pq_is_full(vee_priority_queue_t *pq);

/*
 * Is the priority queue has not been initialized?
 */
int vee_pq_not_inited(vee_priority_queue_t *pq);

/*
 * Is the priority queue node is null?
 */
int vee_pq_node_is_null(vee_priority_queue_node_t *node);

/*
 * Get the number of nodes in priority queue.
 */
unsigned int vee_pq_get_size(vee_priority_queue_t *pq);

#endif  /* VEE_PRIORITY_QUEUE_H */
