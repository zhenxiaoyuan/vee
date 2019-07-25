/*
 * author   zhenxiaoyuan
 * email    i_zhenhao@163.com
 */

#include "vee.h"
#include "error.h"

#define MAX_BUF     256     /* buffer size for read and write */
#define MAX_LISTEN  8
#define MAX_EPOLL   64

#define PORT_NUM    7777

void set_nonblock(int);

int
main(int argc, char *argv[])
{
    // usage

    // socket declaration
    int sv_sock, cl_sock;                   // server and client socket
    struct sockaddr_in sv_addr, cl_addr;    // server and client address
    socklen_t addr_len;
    int str_len, i;
    char buf[MAX_BUF];

    // server socket init
    sv_sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&sv_addr, sizeof(sv_addr));
    sv_addr.sin_family      = AF_INET;
    sv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sv_addr.sin_port        = htons(PORT_NUM);

    // bind server socket
    if (bind(sv_sock, (struct sockaddr *) &sv_addr, sizeof(sv_addr)) == -1)
        error_exit("bind");

    // start listening
    if (listen(sv_sock, MAX_LISTEN) == -1)
        error_exit("listen");

    // epoll declaration
    struct epoll_event ev_list[MAX_EPOLL];
    struct epoll_event ev;
    int epfd, ready;

    // epoll create
    epfd = epoll_create1(0);

    // set server socket to nonblock
    set_nonblock(sv_sock);
    // add server socket to epoll
    ev.events = EPOLLIN;
    ev.data.fd = sv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sv_sock, &ev);

    while (1) {
        // meaning of every param, especially the last one
        ready = epoll_wait(epfd, ev_list, MAX_EPOLL, -1);
        if (ready == -1) {
            if (errno == EINTR)
                continue;
            else
                error_exit("epoll_wait");
        }

        for (int i = 0; i < ready; i++) {
            // server socket - accept - add client socket to epoll
            if (ev_list[i].data.fd == sv_sock) {
                addr_len = sizeof(cl_addr);
                cl_sock = accept(sv_sock, (struct sockaddr *) &cl_addr, &addr_len);
                // set client socket to nonblock
                set_nonblock(cl_sock);
                ev.events = EPOLLIN | EPOLLET;  // use edge trigger
                ev.data.fd = cl_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cl_sock, &ev);

                log_info("connected client: %d", cl_sock);

            } else {
                // client socket
                while (1) {
                    // client socket - read
                    str_len = read(ev_list[i].data.fd, buf, MAX_BUF);

                    // str_len < 0 - judge EAGAIN
                    // todo: read tlpi/chapter4
                    if (str_len < 0) {
                        if (errno == EAGAIN)
                            break;
                    }
                    // str_len == 0 - read completed - close client socket and delete in epoll
                    else if (str_len == 0) {
                        close(ev_list[i].data.fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ev_list[i].data.fd, NULL);
                        log_info("closed client: %d", ev_list[i].data.fd);

                        break;
                    }
                    // str_len > 0 - write
                    else {
                        write(ev_list[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    // close server socket and epoll fd
    close(sv_sock);
    close(epfd);

    return 0;
}

void set_nonblock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

// todo 
// 0. resolve starving
// 1. thread and threadpool
