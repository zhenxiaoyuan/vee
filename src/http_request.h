#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <time.h>
#include "http.h"

#define VEE_AGAIN   EAGAIN

#define VEE_HTTP_PARSE_INVALID_METHOD   10
#define VEE_HTTP_PARSE_INVALID_REQUEST  11
#define VEE_HTTP_PARSE_INVALID_HEADER   12

// what's these?
#define VEE_HTTP_UNKNOWN                0x0001
#define VEE_HTTP_GET                    0x0002
#define VEE_HTTP_HEAD                   0x0004
#define VEE_HTTP_POST                   0x0008

#define VEE_HTTP_OK                     200
#define VEE_HTTP_NOT_MODIFIED           304
#define VEE_HTTP_NOT_FOUND              404

#define MAX_BUF 8214

typedef struct vee_http_request_s {
    void *root;
    int fd;
    int epfd;
    char buf[MAX_BUF];
    size_t pos, last;
    int state;
    void *request_start;
    void *request_end;
    int method;
    void *method_end;
    void *uri_start;
    void *uri_end;
    void *path_start;
    void *path_end;
    void *query_start;
    void *query_end;
    int http_major;
    int http_minor;

    struct list_head list;
    void *cur_header_key_start;
    void *cur_header_key_end;
    void *cur_header_value_start;
    void *cur_header_value_end;

    void *timer;
} vee_http_request_t;

typedef struct vee_http_out_s {
    int fd;
    int keep_alive;
    time_t mtime;
    int modified;

    int status;
} vee_http_out_t;

typedef struct vee_http_header_s {
    void *key_start, *key_end;
    void *value_start, *value_end;
    list_head list;     // what's this definition?
} vee_http_header_t;

typedef int (*vee_http_header_handler_pt)(vee_http_request_t *r, vee_http_out_t *o, char *data, int len);

typedef struct vee_http_header_handle_s {
    char *name;
    vee_http_header_handler_pt handler;
} vee_http_header_handle_t;

void vee_http_handle_header(vee_http_request_t *r, vee_http_out_t *o);
int vee_http_close_conn(vee_http_request_t *r);

int vee_init_request_t(vee_http_request_t *r, int fd, int epfd, vee_conf_t *cf);    // vee_conf_t defined in util.h
int vee_free_request_t(vee_http_request_t *r);

int vee_init_out_t(vee_http_out_t *o, int fd);
int vee_free_out_t(vee_http_out_t *o);

const char *get_shortmsg_from_status_code(int status_code);

extern vee_http_header_handle_t vee_http_headers_in[];  // what's this?

#endif
