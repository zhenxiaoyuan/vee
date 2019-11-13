#ifndef VEE_REQUEST_H
#define VEE_REQUEST_H

typedef struct vee_request_s {
    int fd;
    int epfd;
} vee_request_t;

vee_request_t *vee_request_init(int fd, int epfd);

#endif  /* VEE_REQUEST_H */
