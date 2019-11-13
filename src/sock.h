#ifndef VEE_SOCK_H
#define VEE_SOCK_H

#include <strings.h>

#define PORT_NUM 7777   // read in cfg file later
#define LISTENQ  8

int vee_listenfd_init(void);
int vee_clt_sock_init(int listenfd);

#endif  /* VEE_SOCK_H */
