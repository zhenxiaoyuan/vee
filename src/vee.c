/*
 *      Author:     Hao Zhen
 *      E-Mail:     i_zhenhao@163.com
 */

#include <sys/socket.h>     /* used for socket */
#include <netinet/in.h>     /* used for struct sockaddr_in */
#include <fcntl.h>          /* used for nonblock */
#include <unistd.h>         /* used for read and write */
#include <strings.h>
#include <errno.h>

#include "epoll.h"
#include "error.h"
#include "timer.h"
/*#include "priority_queue.h"*/
#include "request.h"

#define PORT_NUM 7777   // read in cfg file later
#define MAX_BUF     256     /* buffer size for read and write */
#define LISTENQ  8
#define VEE_DEFAULT_TIMEOUT 500         /* Milliseconds */

/*extern struct epoll_event *ev_list;*/
/*extern vee_priority_queue_t *pq;*/

void make_socket_non_block(int socket);

int main(int argc, char **argv) {
    // TODO
    // read_cfg_file()
    // 1. listen_port
    // 2. thread_num
    // 3. ...

    // socket declaration
    // TODO: change svr_sock to listenfd when extract later
    int svr_sock, clt_sock;                 // socket
    struct sockaddr_in svr_addr, clt_addr;  // address
    socklen_t addr_len;     // use in accept
    int str_len;     // str_len is the length in IO operation, move into IO operation.
    char buf[MAX_BUF];  // read and write buffer, move into IO operation.

    // socket init
    // add socket reuse
    svr_sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family      = AF_INET;
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    svr_addr.sin_port        = htons(PORT_NUM);

    // bind
    if (bind(svr_sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1)
        err_exit("[main] bind error");

    // listen
    if (listen(svr_sock, LISTENQ) == -1)
        err_exit("[main] listen error");
    //
    // set_nonblock
    make_socket_non_block(svr_sock);
    //
    // epoll
    // init
    int epfd, timer;
    epfd = vee_epoll_create();
    // TODO: make to vee_epoll_init()
//    extern struct epoll_event *ev_list;
    struct epoll_event ev;
    //
    // init request
    vee_conn_t *svr_conn = vee_conn_init(svr_sock, epfd);
    /*vee_http_request_t *request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));*/
    /*vee_init_request_t(request, fd);*/

    // add listenfd to epoll
    ev.events = EPOLLIN | EPOLLET;
    //ev.data.fd = svr_sock;
    ev.data.ptr = (void *)svr_conn;
    // set ev.data.fd in vee_init_request_t()
    // what's the diff between ev.data.fd and ev.data.ptr->fd

    // change svr_sock to listenfd
    vee_epoll_add(epfd, svr_sock, &ev);

    // thread pool used to do requests
    // init thread pool
    //vee_threadpool_init();  // TODO: threadpool
    // init timer, priority queue included in timer
    vee_timer_init();
    vee_timer_init();

    //
    while (1) {
        // find the minest timer for epoll_wait
        timer = vee_find_timer();
        // epoll_wait
        int ready = vee_epoll_wait(epfd, ev_list, VEE_MAXEVENTS, timer);
        //int ready = epoll_wait(epfd, ev_list, VEE_MAXEVENTS, -1/*time*/);
        // expire timers
        vee_expire_timers();

        for (int i = 0; i < ready; i++) {
            //vee_http_request_t request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));
            //vee_http_request_t request = (vee_http_request_t *)ev_list[i].data.ptr;
            //svr_sock = request->fd; // use infd at here later
            vee_conn_t *conn = (vee_conn_t *)ev_list[i].data.ptr;
            // fd == listenfd
            if (svr_sock == conn->fd) {
                // accept clt_sock
                addr_len = sizeof(clt_addr);
                clt_sock = accept(svr_sock, (struct sockaddr *) &clt_addr, &addr_len);
                // set clt_sock to non block
                make_socket_non_block(clt_sock);
                // epoll_add
                // is this influence the epoll_event when listenfd?
                // maybe no influence: ev_list changed in kernel

                // init request
                vee_conn_t *clt_conn = vee_conn_init(clt_sock, epfd);
                // divide to conn and request(include conn and others)
                //vee_http_request_t *request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));
                //vee_init_request_t(request);

                // add listenfd to epoll
                ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                /*ev.data.fd = clt_sock;*/
                ev.data.ptr = (void *)clt_conn;
                //ev.data.ptr = (void *)request;  // what's the structure of ev.data ?

                vee_epoll_add(epfd, clt_sock, &ev);

                vee_add_timer(clt_conn, VEE_DEFAULT_TIMEOUT);

            } else {
                // exclude error event, !EPOLLIN is ok, why use ERR and HUP?
                if ((ev_list[i].events & EPOLLERR) ||
                    (ev_list[i].events & EPOLLHUP) ||
                    (!(ev_list[i].events & EPOLLIN))) {
                    //log_err(); // TODO: log
                    // err_msg();
                    close(conn->fd);
                    continue;
                }
                // add to threadpool to do request
                //vee_threadpool_add(tp, do_request, events[i].data.ptr);
                while (1) {
                    // client socket - read
                    // temp variable?
                    str_len = read(conn->fd, buf, MAX_BUF);

                    // str_len < 0 - judge EAGAIN
                    // todo: read tlpi/chapter4
                    if (str_len == -1) {
                        if (errno == EAGAIN) {
                            ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                            ev.data.ptr = (void *)conn;
                            vee_epoll_mod(epfd, conn->fd, &ev);
                            break;
                        }
                    }
                    // str_len == 0 - read completed - close client socket and delete in epoll
                    else if (str_len == 0) {
                        //vee_epoll_del(epfd, ev_list[i].data.fd, (struct epoll_event *)NULL);
                        close(conn->fd);
                        //ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                        //ev.data.fd = ev_list[i].data.fd;
                        //vee_epoll_mod(epfd, ev_list[i].data.fd, &ev);
                        //log_info("closed client: %d", ev_list[i].data.fd);

                        break;
                    }
                    // str_len > 0 - write
                    else {
                        write(conn->fd, buf, str_len);
                    }
                }

            }
        }
    }

    close(svr_sock);
    close(epfd); // necessary? - yes

    return 0;
}

// move to util.h later
void make_socket_non_block(int socket)
{
    int flags;

    if ((flags = fcntl(socket, F_GETFL, 0)) == -1)
        err_exit("[make_socket_non_block] fcntl F_GETFL error");

    flags |= O_NONBLOCK;
    if ((flags = fcntl(socket, F_SETFL, flags)) == -1)
        err_exit("[make_socket_non_block] fcntl F_SETFL error");
}


