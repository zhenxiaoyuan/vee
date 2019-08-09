
2 epoll - event poll


#include <sys/epoll.h>

2.1 create
int epoll_create(int size);
int epoll_create1();
return fd on success, -1 on error.
create1
2.2 ctl
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);
return 0 on success, -1 on error.
*edge trigger*
*oneshot*
2.2.1 add
int epoll_ctl(int epfd, EPOLL_CTL_ADD, int fd, struct epoll_event *ev);
return EEXIST on error.
2.2.2 mod
int epoll_ctl(int epfd, EPOLL_CTL_MOD, int fd, struct epoll_event *ev);
return ENOENT on error.
2.2.3 del
int epoll_ctl(int epfd, EPOLL_CTL_DEL, int fd, NULL-ignore);
return ENOENT on error.
close(fd) -> remove from all epoll interest lists.
2.3 wait
int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);
return number of ready fd, 0 on timeout, -1 on error.
timeout
-1 = block until occur
0 = nonblock check
>0 block for up to timeout
2.4 close and release sources (don't necessary in this program , see q 3)

1 - what's the difference of fd between interest list and epoll_data_t.fd? - same
in this program, epoll_data_t.fd get the value in request_http_init.
2 - why EPOLLONESHOT? - active just once
ure epoll__mod to reactive in the interest list.
3 - why don't use epoll_del in this program?
close(fd) -> remove from all epoll interest lists. -
what will happen if dup() or fork() fd for one open fd? - open fd will removed only after all fd closed.
just close the fd when request end.
attention: can't use epoll_add to reactive because it's already in the interest list
4 - how about multithread?
epoll_ctl and epoll_wait can used in multithread program.
5 - see the page 95
fd - open fd - i-node
6 - list in the kernel space, but how to relate list and epoll_event?
7 - what's the difference between level-triggered and edge-triggered?
trigger every time - trigger once
                     read all once
nonblock is for read / write, prevent io time too long
read /write once   - while { read / write } at once
read /write, 0 on EOF and close request, -1 on errno, > 0 on length
8 - what's the advantage of edge triggered?
divide data recv and process using EAGAIN and EBLOCK...
less epoll_event in interest_list
9 - starving in edge triggered?
use timer to prevent starvation
think: find_timer or -1 for timeout in epoll_wait
TODO: deep in nonblock and block io
