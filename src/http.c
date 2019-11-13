#include <errno.h>
#include <unistd.h>

#include "http.h"
#include "timer.h"
#include "epoll.h"

#define MAX_BUF     256     /* buffer size for read and write */

void do_request(void *arg)
{
    vee_request_t *r;
    r = (vee_request_t *)arg;

    /* Move to IO operation */
    int str_len;
    char buf[MAX_BUF];
    struct epoll_event ev;

    while (1) {
        str_len = read(r->fd, buf, MAX_BUF);

        // str_len < 0 - Judge EAGAIN
        // TODO: read tlpi/chapter4
        if (str_len == -1) {
            if (errno == EAGAIN) {
                ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                ev.data.ptr = (void *)r;
                vee_epoll_mod(r->epfd, r->fd, &ev);
                vee_add_timer(r, VEE_TIMER_TIMEOUT);
                break;
            }
        }

        // str_len == 0 - read completed - close client socket and delete in epoll
        else if (str_len == 0) {
            close(r->fd);

            break;
        }

        // str_len > 0 - write
        else {
            write(r->fd, buf, str_len);
        }
    }
}
