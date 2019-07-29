#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <math.h>
#include <time.h>
#include <unistd.h>
#include "http_request.h"
#include "error.h"

static int vee_http_process_ignore(vee_http_request_t *r, vee_http_out_t *o, char *data, int len);
static int vee_http_process_connection(vee_http_request_t *r, vee_http_out_t *o, char *data, int len);
static int vee_http_process_if_modified_since(vee_http_request_t *r, vee_http_out_t *o, char *data, int len);

vee_http_header_handle_t vee_http_headers_in[] = {
    {"Host", vee_http_process_ignore},
    {"Connection", vee_http_process_connection},
    {"If-Modified-Since", vee_http_process_if_modified_since},
    {"", vee_http_process_ignore}
};

int vee_init_request_t(vee_http_request_t *r, int fd, int epfd, vee_conf_t *cf) {
    r->fd = fd;
    r->epfd = epfd;
    r->pos = r->last = 0;
    r->state = 0;
    r->root = cf->root;
    INIT_LIST_HEAD(&(r->list));

    return VEE_OK;
}

int vee_free_request_t(vee_http_request_t *r) {
    // TODO ???
    (void) r;
    return VEE_OK;
}

int vee_init_out_t(vee_http_out_t *o, int fd) {
    o->fd = fd;
    o->keep_alive = 0;
    o->modified = 1;
    o->status = 0;

    return VEE_OK;
}

int vee_free_out_t(vee_http_out_t *o) {
    // TODO ???
    (void) o;
    return VEE_OK;
}

void vee_http_handle_header(vee_http_request_t *r, vee_http_out_t *o) {
    list_head *pos;
    vee_http_header_t *hd;
    vee_http_header_handle_t *header_in;
    int len;

    list_for_each(pos, &(r->list)) {
        hd = list_entry(pos, vee_http_header_t, list);

        for (header_in = vee_http_headers_in;
                strlen(header_in->name) > 0;
                header_in++) {
            // what's this doing for?
            // strncmp ?
            if (strncmp(hd->key_start, header_in->name, hd->key_end - hd->key_start) == 0) {
                len = hd->value_end = hd->value_start;
                (*(header_in->handler))(r, o, hd->value_start, len);
                break;
            }
        }

        list_del(pos);
        free(hd);
    }
}

int vee_http_close_conn(vee_http_request_t * r) {
    close(r->fd);
    free(r);

    return VEE_OK;
}

static int vee_http_process_ignore(vee_http_request_t *r, vee_http_out_t *o, char *data, int len) {
    (void) r;
    (void) out;
    (void) data;
    (void) len;

    return VEE_OK;
}

static int vee_http_process_connection(vee_http_request_t *r, vee_http_out_t *o, char *data, int len) {
    (void) r;
    // what's strncasecmp?
    if (strncasecmp("keep-alive", data, len) == 0) {
        out->keep_alive = 1;
    }

    return VEE_OK;
}

static int vee_http_process_if_modified_since(vee_http_request_t *r, vee_http_out_t *o, char *data, int len) {
    (void) r;
    (void) len;
}
