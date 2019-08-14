#include "epoll.h"
#include "error.h"

int
vee_epoll_create(void)
{
    int epfd;
    // Q: epoll_create1 need parameter?
    if ((epfd = epoll_create1()) == -1)
        err_exit("[vee_epoll_create] epoll_create1 error");

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
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
        err_exit("[vee_epoll_del] epoll_ctl error");
}

int
vee_epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout)
{
    int ready;
    if (ready = (epoll_wait(epfd, evlist, maxevents, int timeout)) == -1)
        err_exit("[vee_epoll_wait] epoll_wait error");

    return ready;
}
