#include "priority_queue.h"

int vee_pq_init(vee_pq_t *vee_pq, vee_pq_comparator_pt comp, size_t size) {
    vee_pq->pq = (void **)malloc(sizeof(void *) * (size+1));
    if (!vee_pq->pq) {
        log_err("vee_pq_init: malloc failed");
        return -1;
    }

    vee_pq->nalloc = 0;
    vee_pq->size = size + 1;
    vee_pq->comp = comp;

    return VEE_OK;
}

int vee_pq_is_empty(vee_pq_t *vee_pq) {
    return (vee_pq-nalloc == 0) ? 1 : 0;
}

size_t vee_pq_size(vee_pq_t *vee_pq) {
    return vee_pq->nalloc;
}

void *vee_pq_min(vee_pq_t *vee_pq) {
    if (vee_pq_is_empty(vee_pq)) {
        return NULL;
    }

    return vee_pq->pq[1];  // why return the index 1?
}

static int resize(vee_pq_t *vee_pq, size_t new_size) {
    if (new_size <= vee_pq_nalloc) {
        log_err("resize: new_size to small");
        return -1;
    }

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if (!new_ptr) {
        log_err("resize: malloc failed");
        return -1;
    }

    memcpy(new_ptr, vee_pq->pq, sizeof(void *) * (vee_pq->nalloc + 1));
    free(vee_pq->pq);
    vee_pq->pq = new_ptr;
    vee_pq->size = new_size;

    return VEE_OK;
}

static void exch(vee_pq_t *vee_pq, size_t i, size_t j) {
    void *tmp = vee_pq->pq[i];
    vee_pq->pq[i] = vee_pq->pq[j];
    vee_pq->pq[j] = tmp;
}

// what about k?
static void swim(vee_pq_t *vee_pq, size_t k) {
    while (k > 1 && vee_pq->comp(vee_pq->pq[k], vee_pq->pq[k/2])) {
        exch(vee_pq, k, k/2);
        k /= 2;
    }
}

// what's this func mean?
static size_t sink(vee_pq_t *vee_pq, size_t k) {
    size_t j;
    size_t nalloc = vee_pq->nalloc;

    while (2*k <= nalloc) {
        j = 2*k;
        if (j < nalloc && vee_pq->comp(vee_pq->pq[j+1], vee_pq->pq[j])) j++;
        if (!vee_pq->comp(vee_pq->pq[j], vee_pq->pq[k])) break;
        exch(vee_pq, j, k);
        k = j;
    }

    return k;
}

int vee_pq_delmin(vee_pq_t *vee_pq) {
    if (vee_pq_is_empty(vee_pq)) {
        return VEE_OK;
    }

    exch(vee_pq, 1, vee_pq->nalloc);
    vee_pq->nalloc--;
    sink(vee_pq, 1);
    if (vee_pq->nalloc > 0 && vee_pq->nalloc <= (vee_pq->size - 1)/4) {
        if (resize(vee_pq, vee_pq->size / 2) < 0) {
            return -1;
        }
    }

    return VEE_OK;
}

int vee_pq_insert(vee_pq_t *vee_pq, void *item) {
    if (vee_pq->nalloc + 1 == vee_pq->size) {
        if (resize(vee_pq, vee_pq->size * 2) < 0) {
            return -1;
        }
    }

    vee_pq->pq[++vee_pq->nalloc] = item;
    swim(vee_pq, vee_pq->nalloc);

    return VEE_OK;
}

int vee_pq_sink(vee_pq_t *vee_pq, size_t i) {
    return sink(vee_pq, i);
}
