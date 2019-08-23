#include <stdlib.h>

#include "request.h"
#include "error.h"

vee_conn_t *vee_conn_init(int fd, int epfd)
{
    vee_conn_t *conn;
    if ((conn = (vee_conn_t *)malloc(sizeof(vee_conn_t))) == NULL)
        err_exit("[vee_conn_init] malloc vee_conn_t error");

    conn->fd = fd;
    conn->epfd = epfd;

    return conn;
}
