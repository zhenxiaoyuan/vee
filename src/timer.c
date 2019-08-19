#include <sys/time.h>

#include "timer.h"
#include "priority_queue.h"

/*// timer comparation func, using when init the priority queue.*/
/*static int timer_comp(void *ti, void *tj) {*/
    /*vee_timer_node *timeri = (vee_timer_node *)ti;*/
    /*vee_timer_node *timerj = (vee_timer_node *)tj;*/

    /*return (timeri->key < timerj->key) ? 1 : 0;*/
/*}*/

/*vee_pq_t vee_timer;*/
/*size_t vee_current_msec;*/

/*// update current time*/
/*static void vee_time_update() {*/
    /*struct timeval tv;*/
    /*int rc;*/

    /*rc = gettimeofday(&tv, NULL);*/
    /*check(rc == 0, "vee_time_update: gettimeofday error");*/

    /*vee_current_msec = tv.tv_sec * 1000 + tv.tv_usec / 1000;*/
    /*debug("in vee_time_update, time = %zu", vee_current_msec);*/
/*}*/

/*int vee_timer_init() {*/
    /*int rc;*/
    /*rc = vee_pq_init(&vee_timer, timer_comp, VEE_PQ_DEFAULT_SIZE);*/
    /*check(rc == VEE_OK, "vee_pq_init error");*/

    /*vee_time_update();*/
    /*return VEE_OK;*/
/*}*/

/*int vee_find_timer() {*/
    /*vee_timer_node *timer_node;*/
    /*int time = VEE_TIMER_INFINITE;*/
    /*int rc;*/

    /*while (!vee_pq_is_empty(&vee_timer)) {*/
        /*debug("vee_find_timer");*/
        /*vee_time_update();*/
        /*timer_node = (vee_timer_node *)vee_pq_min(&vee_timer);*/
        /*check(timer_node != NULL, "vee_pq_min error");*/

        /*if (timer_node->deleted) {*/
            /*rc = vee_pq_delmin(&vee_timer);*/
            /*check(rc == 0, "vee_pq_delmin");*/
            /*free(time_node);*/
            /*continue;*/
        /*}*/

        /*time = (int) (timer_node->key - vee_current_msec);*/
        /*debug("in vee_find_timer, key = %zu, cur = %zu",*/
                /*timer_node->key,*/
                /*vee_current_msec);*/
        /*time = (time > 0 ? time : 0);*/
        /*break;*/
    /*}*/

    /*return time;*/
/*}*/

/*void vee_handle_expire_timers() {*/
    /*debug("in vee_handle_expire_timers");*/
    /*vee_timer_node *timer_node;*/
    /*int rc;*/

    /*while (!vee_pq_is_empty(&vee_timer)) {*/
        /*debug("vee_handle_expire_timers, size = %zu", vee_pq_size(&vee_timer));*/
        /*vee_time_update();*/
        /*timer_node = (vee_timer_node *)vee_pq_min(&vee_timer);*/
        /*check(timer_node != NULL, "vee_pq_min error");*/

        /*if (timer_node->deleted) {*/
            /*rc = vee_pq_delmin(&vee_timer);*/
            /*check(rc == 0, "vee_handle_expire_timers: vee_pq_delmin error");*/
            /*free(timer_node);*/
            /*continue;*/
        /*}*/

        /*if (timer_node->key > vee_current_msec) {*/
            /*return;*/
        /*}*/

        /*if (timer_node->handler) {*/
            /*timer_node->handler(timer_node->rq);*/
        /*}*/
        /*rc = vee_pq_delmin(&vee_timer);*/
        /*check(rc == 0, "vee_handle_expire_timers: vee_pq_delmin error");*/
        /*free(timer_node);*/
    /*}*/
/*}*/

/*void vee_add_timer(vee_http_request_t *rq, size_t timeout, timer_handler_pt handler) {*/
    /*int rc;*/
    /*vee_timer_node *timer_node = (vee_timer_node *)malloc(sizeof(vee_timer_node));*/
    /*check(timer_node != NULL, "vee_add_timer: malloc error");*/

    /*vee_time_update();*/
    /*rq->timer = timer_node;*/
    /*timer_node->key = vee_current_msec + timeout;*/
    /*debug("in vee_add_timer, key = %zu", timer_node->key);*/
    /*timer_node->deleted = 0;*/
    /*timer_node->handler = handler;*/
    /*timer_node->rq = rq;*/

    /*rc = vee_pq_insert(&vee_timer, timer_node);*/
    /*check(rc == 0, "vee_add_timer: vee_pq_insert error");*/
/*}*/

/*void vee_del_timer(vee_http_request_t *rq) {*/
    /*debug("in vee_del_timer");*/
    /*vee_time_update();*/
    /*vee_timer_node *timer_node = rq->timer;*/
    /*check(timer_node != NULL, "vee_del_timer: rq->timer is NULL");*/

    /*timer_node->deleted = 1;*/
/*}*/
