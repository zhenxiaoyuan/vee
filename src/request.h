#ifndef VEE_REQUEST_H
#define VEE_REQUEST_H

typedef struct vee_conn_s {
    int fd;
    int epfd;   // Necessary?
} vee_conn_t;

vee_conn_t *vee_conn_init(int fd, int epfd);

#endif  /* VEE_REQUEST_H */
