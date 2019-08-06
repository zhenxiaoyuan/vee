//#ifndef EPOLL_H
//#define EPOLL_H
//
//#include <sys/epoll.h>
//
//#define MAXEVENTS 1024
//
//int vee_epoll_create(int flags);
//void vee_epoll_add(int epfd, int fs, struct epoll_event *event);
//void vee_epoll_mod(int epfd, int fs, struct epoll_event *event);
//void vee_epoll_del(int epfd, int fs, struct epoll_event *event);
//int vee_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
//
//#endif
//
//
#ifndef VEE_EPOLL_H
#define VEE_EPOLL_H

#include <sys/epoll.h>

// some define

// TODO
int vee_epoll_create();
void vee_epoll_add();
void vee_epoll_mod();
void vee_epoll_del(); // never used in this program, maybe.
void vee_epoll_wait();

#endif
