//
// Latest edit by TeeKee on 2017/7/23.
//

#ifndef VEE_HTTP_REQUEST_H
#define VEE_HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
//#include "util.h"
#include "list.h"

#define VEE_AGAIN EAGAIN

#define VEE_HTTP_PARSE_INVALID_METHOD        10
#define VEE_HTTP_PARSE_INVALID_REQUEST       11
#define VEE_HTTP_PARSE_INVALID_HEADER        12

#define VEE_HTTP_UNKNOWN                     0x0001
#define VEE_HTTP_GET                         0x0002
#define VEE_HTTP_HEAD                        0x0004
#define VEE_HTTP_POST                        0x0008

#define VEE_HTTP_OK                          200
#define VEE_HTTP_NOT_MODIFIED                304
#define VEE_HTTP_NOT_FOUND                   404
#define MAX_BUF                              8124

typedef struct vee_http_request_s {
    char* root;
    int fd;
    int epfd;
    char buff[MAX_BUF];
    size_t pos;
    size_t last;
    int state;

    void* request_start;
    void* method_end;
    int method;
    void* uri_start;
    void* uri_end;
    void* path_start;
    void* path_end;
    void* query_start;
    void* query_end;
    int http_major;
    int http_minor;
    void* request_end;

    struct list_head list;    // 存储请求头，list.h中定义了此结构

    void* cur_header_key_start;
    void* cur_header_key_end;
    void* cur_header_value_start;
    void* cur_header_value_end;
    void* timer;
} vee_http_request_t;

typedef int (*handler_ptr)(vee_http_request_t *r);

typedef struct vee_http_out_s {
    int fd;
    int keep_alive;
    time_t mtime;
    int modified;
    int status;
} vee_http_out_t;

typedef struct vee_http_header_s {
    void* key_start;
    void* key_end;
    void* value_start;
    void* value_end;
    struct list_head list;
} vee_http_header_t;

typedef int (*vee_http_header_handler_pt)(vee_http_request_t* request, vee_http_out_t* out, char* data, int len);

typedef struct vee_http_header_handle_s {
    char* name;
    vee_http_header_handler_pt handler;    // 函数指针
} vee_http_header_handle_t;

extern vee_http_header_handle_t vee_http_headers_in[];

void vee_http_handle_header(vee_http_request_t* request, vee_http_out_t* out);
int vee_http_close_conn(vee_http_request_t *r);
vee_http_request_t *vee_http_request_init(int fd, int epfd, char *path);
int vee_http_out_init(vee_http_out_t* out, int fd);
const char* get_shortmsg_from_status_code(int status_code);

#endif
