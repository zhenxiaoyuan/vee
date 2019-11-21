#ifndef VEE_HTTP_REQUEST_H
#define VEE_HTTP_REQUEST_H

typedef struct vee_http_request_s {
    int fd;
    int epfd;
} vee_http_request_t;

vee_http_request_t *vee_http_request_init(int fd, int epfd);

#endif  /* VEE_HTTP_REQUEST_H */
