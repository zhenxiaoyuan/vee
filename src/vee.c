/*
 *      Author:     Hao Zhen
 *      E-Mail:     i_zhenhao@163.com
 */

#define PORT_NUM 7777   // read in cfg file later

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
    svr_sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family      = AF_INET;
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    svr_addr.sin_port        = htons(PORT_NUM);

    // bind
    if (bind(svr_sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
        err_sys("[main] bind error");
        // TODO error.h and .c
    }

    // listen
    if (listen(svr_sock, LISTENQ) == -1) {
        err_sys("[main] listen error");
    }
    //
    // set_nonblock
    make_socket_non_block(svr_sock);
    //
    // epoll
    // init
    int epfd;
    struct epoll_event ev_list[MAX_EPOLL];
    struct epoll_event ev;
    // create
    epfd = epoll_create1(0);    // TODO: use macro to control events_num
    //
    // init request
    vee_http_request_t *request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));
    vee_init_request_t(request, fd);

    // add listenfd to epoll
    ev.events = EPOLLIN | EPOLLET;    // if listenfd need EPOLLET and EPOLLONESHOT ?
    ev.data.ptr = (void *)request;  // what's the structure of ev.data ?
    // set ev.data.fd in vee_init_request_t()
    // what's the diff between ev.data.fd and ev.data.ptr->fd

    vee_epoll_add(epfd, svr_sock, &ev);
    // TODO: error control?

    // thread pool used to do requests
    // init thread pool
    vee_threadpool_init();  // TODO: threadpool
    // init timer, priority queue included in timer
    vee_timer_init();       // TODO: timer
    //
    while (1) {
        // find the minest timer for epoll_wait
        time = vee_find_timer();
        // epoll_wait
        ready = vee_epoll_wait(epfd, ev_list, MAX_EPOLL, time);
        // TODO: ready error control
        // expire timers
        vee_handle_expire_timers();
        //
        for (int i = 0; i < ready; i++) {
            //vee_http_request_t request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));
            vee_http_request_t request = (vee_http_request_t *)ev_list[i].data.ptr;
            svr_sock = request->fd; // use infd at here later
            // fd == listenfd
            if (svr_sock == ev_list[i].data.fd) {
                // accept clt_sock
                clt_sock = accept(svr_sock, (struct sockaddr *) &cltaddr, &addrlen);
                // set clt_sock to non block
                make_socket_non_block(clt_sock);
                // epoll_add
                // is this influence the epoll_event when listenfd?
                // maybe no influence: ev_list changed in kernel

                // init request
                vee_http_request_t *request = (vee_http_request_t *)malloc(sizeof(vee_http_request_t));
                vee_init_request_t(request);

                // add listenfd to epoll
                ev.events = EPOLLIN | EPOLLET | EPOLLONSHOT; // why onshot?
                ev.data.ptr = (void *)request;  // what's the structure of ev.data ?

                vee_epoll_add(epfd, clt_sock, &ev);
                vee_add_timer()
            } else {
                // exclude error event, !EPOLLIN is ok, why use ERR and HUP?
                if ((ev_list[i].events & EPOLLERR) ||
                    (ev_list[i].events & EPOLLHUP) ||
                    (!(ev_list[i].events & EPOLLIN))) {
                    log_err(); // TODO: log
                    close(fd);
                    continue;
                }
                // add to threadpool to do request
                vee_threadpool_add(tp, do_request, events[i].data.ptr);

            }
        }
    }
}

// move to util.h later
int make_socket_non_block(int socket) {
    // TODO
}


