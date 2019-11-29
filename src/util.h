#ifndef VEE_UTIL_H
#define VEE_UTIL_H

#define BUF_LEN 1024
#define DELIM   " "

typedef struct vee_conf_s {
    char    root[BUF_LEN];
    int     port;
    int     thread_num;
} vee_conf_t;

vee_conf_t *vee_read_conf(char *filename);

#endif  /* VEE_UTIL_H */
