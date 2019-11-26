#ifndef HTTP_H
#define HTTP_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "http.h"

#define MAXLINE 8192
#define SHORTLINE 512

// 用key-value表示mime_type_t
typedef struct mime_type{
    const char *type;
    const char *value;
}mime_type_t;

void do_request(void *arg);

#endif
