#include <stdlib.h>

#include "http_request.h"
#include "error.h"

vee_http_request_t *vee_http_request_init(int fd, int epfd)
{
    vee_http_request_t *r;
    if ((r = (vee_http_request_t *)malloc(sizeof(vee_http_request_t))) == NULL)
        err_exit("[vee_http_request_init] malloc vee_http_request_t error");

    r->fd = fd;
    r->epfd = epfd;

    return r;
}

