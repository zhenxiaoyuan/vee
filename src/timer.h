#ifndef VEE_TIMER_H
#define VEE_TIMER_H

#include "http_request.h"
#include "priority_queue.h"

#define VEE_TIMER_TIMEOUT 500         /* Milliseconds */

vee_priority_queue_t    *pq;
unsigned long           current_msec;     /* Milliseconds */

void vee_timer_init(void);
int  vee_find_timer(void);
void vee_expire_timers(void);
void vee_add_timer(vee_http_request_t *r, unsigned long timeout, handler_ptr handler);
void vee_timer_del(vee_http_request_t *r);

#endif  /* VEE_TIMER_H */
