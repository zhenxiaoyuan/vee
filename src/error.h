#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define VEE_OK     0
#define VEE_ERROR  -1

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// todo: use err_num in threads
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s %s - %s:%d: errno: %s) " M "\n", __DATE__, __TIME__,  __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s %s - %s:%d) " M "\n", __DATE__,  __TIME__, __FILE__, __LINE__, ##__VA_ARGS__)

#define error_msg(M, ...) log_err(M "\n", ##__VA_ARGS__)

#define error_exit(M, ...) { log_err(M "\n", ##__VA_ARGS__); exit(1); }

// TODO - check

#endif