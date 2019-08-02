#ifndef UTIL_H
#define UTIL_H

#define LISTENQ         1024

#define BUFLEN          8192

#define DELIM           "="

#define VEE_CONF_OK     0
#define VEE_CONF_ERROR  100

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct vee_conf_s {
    void *root;
    int port;
    int thread_num;
} vee_conf_t;

int open_listenfd(int port);
int make_socket_non_blocking(int fd);

int read_conf(char *filename, vee_conf_t *cf, char *buf, int len);

#endif
