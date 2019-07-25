#ifndef HTTP_H
#define HTTP_H

#include <strings.h>
#include <stdint.h>
#include "rio.h"
#include "list.h"
#include "dbg.h"
#include "util.h"
#include "http_request.h"

#define MAXLINE     8192
#define SHORTLINE   512

typedef struct mime_type_s {
    const char *type;
    const char *value;
} mime_type_t;

void do_request(void *infd);

#endif
