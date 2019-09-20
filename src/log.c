#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "error.h"

#define BUF_SIZE 500

static void
output_log(int level, const char *msg)
{
    const char *level_map[] = { "Error", "Info", "Debug" };

    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        err_exit("[vee_output_log] gettimeofday error");

    struct tm *tm;
    if ((tm = localtime(&tv.tv_sec)) == NULL)
        err_exit("[vee_output_log] localtime error");

    char time_text[BUF_SIZE];
    int off;
    if (off = strftime(time_text, BUF_SIZE, "%Y %b %d %H:%M:%S", tm) == 0)
        err_exit("[vee_output_log] strftime error");

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE-off, "[%s][%s] %s\n", level_map[level], time_text, msg);

    fputs(buf, stdout);
    fflush(stdout);

    // TODO: Print to log file.
}

void
logger(int level, const char *fmt, ...)
{
    va_list ap;
    char msg[BUF_SIZE];

    va_start(ap, fmt);
    vsnprintf(msg, BUF_SIZE, fmt, ap);
    va_end(ap);

    output_log(level, msg);
}
