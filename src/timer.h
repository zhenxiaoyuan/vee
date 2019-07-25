#ifndef VEE_TIMER_H
#define VEE_TIMER_H

#include "priority_queue.h"
#include "http_request.h"

#define VEE_TIMER_INIFNITE -1
#define TIMEOUT_DEFAULT 500

typedef int (*timer_handler_pt)(vee_http_request_t *rq);

typedef struct vee_timer_node_s {
    size_t  key;    // what's this?
    int     deleted;
    timer_handler_pt handler;
    vee_http_request_t *rq;
} vee_timer_node;

int vee_timer_init();
int vee_find_timer();
void vee_handler_expire_timers();

extern vee_pq_t vee_timer;
extern size_t vee_current_msec;

void vee_add_timer(vee_http_request_t *rq, size_t timeout, timer_handler_pt handler);
void vee_del_timer(vee_http_request_t *rq);

#endif
