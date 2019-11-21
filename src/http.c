#include <errno.h>
#include <unistd.h>

/* Delete later */
#include <stdio.h>
#include <stdlib.h>

#include "http.h"
#include "timer.h"
#include "epoll.h"
#include "rio.h"

#define MAX_BUF     256     /* buffer size for read and write */

void do_request(void *arg)
{
    vee_http_request_t *r;
    r = (vee_http_request_t *)arg;

    /* Move to IO operation */
    int str_len;
    char buf[MAX_BUF];
    struct epoll_event ev;

    while (1) {
        str_len = read(r->fd, buf, MAX_BUF);

        /* error or EAGAIN | EWOULDBLOCK */
        if (str_len < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
                goto err;
        }
        /* Client disconnect or end-of-file */
        else if (str_len == 0) {
            goto err;
        }
        /* Do request */
        else {
            rio_writen(r->fd, buf, str_len);
        }
    }

    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    ev.data.ptr = (void *)r;
    vee_epoll_mod(r->epfd, r->fd, &ev);
    vee_add_timer(r, VEE_TIMER_TIMEOUT);

    return;

err:
    close(r->fd);
    free(r);
}
