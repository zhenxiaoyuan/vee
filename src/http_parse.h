#ifndef VEE_HTTP_PARSE_H
#define VEE_HTTP_PARSE_H

#include <stdint.h>     /* use for uint32_t */

#include "http_request.h"

#define CR '\r'
#define LF '\n'

#define vee_str3_cmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define vee_str3Ocmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define vee_str4cmp(m, c0, c1, c2, c3)                                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

// http请求行解析
int vee_http_parse_request_line(vee_http_request_t *request);
// http请求体解析
int vee_http_parse_request_body(vee_http_request_t *request);

#endif
