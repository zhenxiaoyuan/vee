#include <sys/socket.h>     /* used for socket */
#include <netdb.h>          /* used for getaddrinfo */
#include <fcntl.h>          /* used for nonblock */
#include <string.h>         /* used for bzero */
#include <unistd.h>         /* used for close */
#include <stdio.h>          /* used for sprintf */

#include "sock.h"
#include "error.h"
#include "log.h"

static void make_sock_non_block(int sock);

int vee_listenfd_init(int port, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char port_str[PORT_LEN];
    int listenfd;
    int optval = 1;

    /* Default port 7777 */
    port = ((port >= 65535) || (port <= 1024)) ? 7777 : port;
    sprintf(port_str, "%d", port);

    bzero(&hints, sizeof(hints));
    hints.ai_flags  = AI_PASSIVE | AI_NUMERICSERV;
                        /* Wildcard IP address; service name is numeric */
    hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
    hints.ai_socktype   = SOCK_STREAM;  /* TCP */
    hints.ai_protocol   = 0;
    hints.ai_addrlen    = 0;
    hints.ai_canonname  = NULL;
    hints.ai_addr       = NULL;
    hints.ai_next       = NULL;

    if (getaddrinfo(NULL, port_str, &hints, &result) != 0)
        err_exit("[vee_listenfd_init] getaddrinfo error.");

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        listenfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (listenfd == -1)
            continue;

        /* Solve the "Address already in use" problem */
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            err_exit("[vee_listenfd_init] setsocket error");

        if (bind(listenfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;          /* Success */

        /* bind failed: close listenfd and try next address */
        close(listenfd);
    }

    if (rp == NULL)
        err_exit("[vee_listenfd_init] no valid result in getaddrinfo");

    if (listen(listenfd, backlog) == -1)
        err_exit("[vee_listenfd_init] listen error");

    char host[NI_MAXHOST], service[NI_MAXSERV];
    int ni_flags = NI_NUMERICHOST | NI_NUMERICSERV;
    if (getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, ni_flags) == 0)
        logger(INFO, "Connection from %s:%s", host, service);
    else
        logger(INFO, "UNKNOWN host:service");

    freeaddrinfo(result);

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


