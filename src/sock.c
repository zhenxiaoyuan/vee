#include <sys/socket.h>     /* used for socket */
#include <netinet/in.h>     /* used for struct sockaddr_in */
#include <fcntl.h>          /* used for nonblock */

#include "sock.h"
#include "error.h"
#include "log.h"

static void make_sock_non_block(int sock);

int vee_listenfd_init(int port)
{
    port = ((port >= 65535) || (port <= 1024)) ? 7777 : port;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Solve the "Address already in use" problem */
    int optval = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        err_exit("[vee_listenfd_init] setsocket error");

    struct sockaddr_in svr_addr;
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family      = AF_INET;
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    svr_addr.sin_port        = htons((unsigned short)port);

    if (bind(listenfd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1)
        err_exit("[vee_listenfd_init] bind error");

    if (listen(listenfd, LISTENQ) == -1)
        err_exit("[vee_listenfd_init] listen error");

    logger(INFO, "Start listening...");

    make_sock_non_block(listenfd);

    return listenfd;
}

int vee_clt_sock_init(int listenfd)
{
    int clt_sock;
    struct sockaddr_in clt_addr;
    socklen_t addr_len = sizeof(clt_addr);

    clt_sock = accept(listenfd, (struct sockaddr *) &clt_addr, &addr_len);

    make_sock_non_block(clt_sock);

    return clt_sock;
}

static void make_sock_non_block(int sock)
{
    int flags;

    if ((flags = fcntl(sock, F_GETFL, 0)) == -1)
        err_exit("[make_sock_non_block] fcntl F_GETFL error");

    flags |= O_NONBLOCK;
    if ((flags = fcntl(sock, F_SETFL, flags)) == -1)
        err_exit("[make_sock_non_block] fcntl F_SETFL error");
}


