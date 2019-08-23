#include <stdlib.h>     /* used for malloc */

#include "epoll.h"
#include "error.h"

int
vee_epoll_create(void)
{
    int epfd;
    if ((epfd = epoll_create1(VEE_EPOLL_FLAGS)) == -1)
        err_exit("[vee_epoll_create] epoll_create1 error");

    ev_list = (struct epoll_event *)malloc(sizeof(struct epoll_event) * VEE_MAXEVENTS);

    return epfd;
}

void
vee_epoll_add(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev) == -1)
        err_exit("[vee_epoll_add] epoll_ctl error");
}

void
vee_epoll_mod(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev) == -1)
        err_exit("[vee_epoll_mod] epoll_ctl error");
}

void
vee_epoll_del(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, ev) == -1)
        err_exit("[vee_epoll_del] epoll_ctl error");
}

int
vee_epoll_wait(int epfd, struct epoll_event *ev_list, int maxevents, int timeout)
{
    int ready;
    if ((ready = epoll_wait(epfd, ev_list, maxevents, timeout)) == -1)
        err_exit("[vee_epoll_wait] epoll_wait error");

    return ready;
}
