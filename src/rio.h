#ifndef VEE_RIO_H
#define VEE_RIO_H

#include <sys/types.h>      /* use for ssize_t and size_t */
#include <unistd.h>         /* use for read and write */

/* TODO: Rewrite
 * 1. handle errno
 * 2. recv/send
 * 3. sendfile
 * 4. TCP_CORK
 * /

/* Read "n" bytes to a descriptor */
ssize_t rio_readn(int fd, char *ptr, size_t n);

/* Write "n" bytes to a descriptor */
ssize_t rio_writen(int fd, const char *ptr, size_t n);

#endif  /* VEE_RIO_H */
