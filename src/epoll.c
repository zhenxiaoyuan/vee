#include "epoll.h"
#include "error.h"  // TODO

int vee_epoll_create(void)
{
    int epfd;
    // TODO: epoll_create1 need parameter?
    if ((epfd = epoll_create1()) == -1) {
        err_sys();  // TODO
    }

    return epfd;
}

void vee_epoll_add(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev) == -1) {
        // operate EEXIST? EEXIST = ?
        err_sys();  // TODO
    }
}

void vee_epoll_mod(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev) == -1) {
        // operate ENOENT?
        err_sys();  // TODO
    }
}

void vee_epoll_del(int epfd, int fd, struct epoll_event *ev)
{
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        // operate ENOENT?
        err_sys();  // TODO
    }
}

int vee_epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout)
{
    int ready;
    if (ready = (epoll_wait(epfd, evlist, maxevents, int timeout)) == -1) {
        err_sys();  // TODO
    }

    return ready;
}
