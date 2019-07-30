#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#include "http.h"
#include "http_parse.h"
#include "error.h"

#define CR '\r'
#define LF '\n'
#define CRLFCRLF "\r\n\r\n"

int vee_http_parse_request_line(vee_http_request_t *r);
int vee_http_parse_request_body(vee_http_request_t *r);

#endif
