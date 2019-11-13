#ifndef VEE_LOG_H
#define VEE_LOG_H

#define ERROR   0
#define INFO    1
#define DEBUG   2

void logger(int level, const char *fmt, ...);

#endif  /* VEE_LOG_H */
