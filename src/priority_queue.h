#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "dbg.h"
#include "error.h"

#define VEE_PQ_DEFAULT_SIZE 10

typedef int (*vee_pq_comparator_pt)(void *pi, void *pj);

typedef struct {
    void **pq;      // why **?
    size_t nalloc;  // the diff between nalloc and size?
    size_t size;
    vee_pq_comparator_pt comp;
} vee_pq_t;

int vee_pq_init(vee_pq_t *vee_pq, vee_pq_comparator_pt comp, size_t size);
int vee_pq_is_empty(vee_pq_t *vee_pq);
size_t vee_pq_size(vee_pq_t *vee_pq);
void *vee_pq_min(vee_pq_t *vee_pq);
int vee_pq_delmin(vee_pq_t *vee_pq);
int vee_pq_insert(vee_pq_t *vee_pq, void *item);

// what's this?
int vee_pq_sink(vee_pq_t *vee_pq, size_t i);

#endif
