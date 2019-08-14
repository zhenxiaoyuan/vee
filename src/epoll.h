/*
 *      Author:     Hao Zhen
 *      E-Mail:     i_zhenhao@163.com
 */
#ifndef VEE_EPOLL_H
#define VEE_EPOLL_H

#include <sys/epoll.h>

#define VEE_EPOLL_FLAGS 0   // TODO: set in config file later
#define VEE_MAXEVENTS   64

/*
 * Create epfd, refer to the new epoll instance
 */
int vee_epoll_create(void);

/* 
 * Register the target file descriptor `fd` on the epoll instance 
 * referred by file descriptor `epfd` and associate the ADD event 
 * `event` with the internal file linked to `fd`.
 */
void vee_epoll_add(int epfd, int fd, struct epoll_event *ev);

/*
 * Change the event `event` associated with the target file
 * descriptor `fd`.
 */
void vee_epoll_mod(int epfd, int fd, struct epoll_event *ev);

/*
 * Remove the target file descriptor `fd` from the epoll instance
 * referred by `epfd`. The `event` is ignored and can be NULL.
 */
void vee_epoll_del(int epfd, int fd, struct epoll_event *ev);

/*
 * Wait for events on the epoll instance referred by the file
 * descriptor `epfd`. The memory area pointed to by `ev_list`
 * will contain the events that will be available for the caller.
 * Up to `maxevents` are returned.
 */
int vee_epoll_wait(int epfd, struct epoll_event *ev_list, int maxevents, int timeout);

#endif  /* VEE_EPOLL_H */
