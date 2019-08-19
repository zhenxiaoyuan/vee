#ifndef VEE_ERROR_H
#define VEE_ERROR_H

#include <stdarg.h>

// TRUE, FALSE is wrong, modify it later
typedef enum { TRUE, FALSE } Boolean;

/*
* Nonfatal error related to a system call.
* Print a message and return.
*/
void err_msg(const char *format, ...);

#ifdef __GNUC__

/* This macro stops 'gcc -Wall' complaining that "control reaches
end of non-void function" if we use the following functions to
terminate main() or some other non-void function. */

#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

/*
* Fatal error related to a system call.
* Print a message and terminate.
*/
void err_exit(const char *format, ...) NORETURN;

/*
* Fatal error related to a system call.
* Doesn't flush standard output before printing the error message.
* Terminates the process by calling _exit() instead of exit().
*/
void _err_exit(const char *format, ...) NORETURN;

/*
* Fatal error unrelated to a system call.
* Print a message and terminate.
*/
void err_exit_en(int errnum, const char *format, ...) NORETURN;

/*
 * Standard error.
 * Print a message and terminate.
 */
void fatal(const char *format, ...) NORETURN;

/*
 * Print string Usage: followed by the
 * formatted output on standard error.
 */
void usage_err(const char *format, ...) NORETURN;

/*
 * Print string Command-line usage error: followed
 * by the formatted output on standard error.
 */
void cmd_err(const char *format, ...) NORETURN;

#endif  /* VEE_ERROR_H */
