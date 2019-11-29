#ifndef VEE_SOCK_H
#define VEE_SOCK_H

#include <strings.h>

#define LISTENQ  8

int vee_listenfd_init(int port);
int vee_clt_sock_init(int listenfd);

#endif  /* VEE_SOCK_H */
