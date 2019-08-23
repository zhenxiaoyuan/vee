#ifndef VEE_TIMER_H
#define VEE_TIMER_H

#include "request.h"
#include "priority_queue.h"

vee_priority_queue_t    *pq;
unsigned long           current_msec;     /* Milliseconds */

void vee_timer_init(void);
int vee_find_timer(void);
void vee_expire_timers(void);
void vee_add_timer(vee_conn_t *r, unsigned long timeout);

#endif  /* VEE_TIMER_H */
