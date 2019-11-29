/*
 *      Author:     Hao Zhen
 *      E-Mail:     i_zhenhao@163.com
 */

#include <unistd.h>     /* use for close */

#include "epoll.h"
#include "timer.h"
#include "threadpool.h"
#include "http_request.h"
#include "http.h"
#include "sock.h"
#include "util.h"
#include "error.h"
#include "log.h"

#define CONF_NAME "vee.conf"

/* defined at epoll.h */
extern struct epoll_event *ev_list;

int main(int argc, char **argv) {

    vee_conf_t *conf = vee_read_conf(CONF_NAME);

    /* Sock */
    int listenfd = vee_listenfd_init(conf->port);

    /* Epoll */
    int epfd, timer;
    epfd = vee_epoll_create(0);

    /* Request */
    vee_http_request_t *r = vee_http_request_init(listenfd, epfd, conf->root);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void *)r;

    vee_epoll_add(epfd, listenfd, &ev);

    /* ThreadPool */
    vee_tp_t *pool = vee_tp_init(conf->thread_num);

    /* Timer */
    vee_timer_init();

    while (1) {
        /* Find the minimum timer for epoll_wait. */
        timer = vee_find_timer();

        int ready = vee_epoll_wait(epfd, ev_list, VEE_MAXEVENTS, timer);

        vee_expire_timers();

        for (int i = 0; i < ready; i++) {
            vee_http_request_t *r = (vee_http_request_t *)ev_list[i].data.ptr;

            if (listenfd == r->fd) {
                int clt_sock = vee_clt_sock_init(listenfd);

                vee_http_request_t *clt_r = vee_http_request_init(clt_sock, epfd, conf->root);

                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                ev.data.ptr = (void *)clt_r;

                vee_epoll_add(epfd, clt_sock, &ev);
                vee_add_timer(clt_r, VEE_TIMER_TIMEOUT, vee_http_close_conn);

            } else {
                if (!(ev_list[i].events & EPOLLIN)) {
                    logger(INFO, "fd %d isn't EPOLLIN, so closed.", r->fd);
                    close(r->fd);
                    continue;
                }

                do_request(r);
                //vee_tp_add(pool, do_request, r);
            }
        }
    }

    close(listenfd);
    close(epfd);

    return 0;
}

