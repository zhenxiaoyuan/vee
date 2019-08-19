#ifndef VEE_TIMER_H
#define VEE_TIMER_H

#define VEE_DEFAULT_TIMEOUT 500

int vee_timer_init(void);
int vee_find_timer(void);
void vee_expire_timers(void);
void vee_add_timer(vee_http_request_t *r, size_t timeout);

#endif  /* VEE_TIMER_H */
