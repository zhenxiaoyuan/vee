#include <stdlib.h>

#include "request.h"
#include "error.h"

vee_request_t *vee_request_init(int fd, int epfd)
{
    vee_request_t *r;
    if ((r = (vee_request_t *)malloc(sizeof(vee_request_t))) == NULL)
        err_exit("[vee_request_init] malloc vee_request_t error");

    r->fd = fd;
    r->epfd = epfd;

    return r;
}

