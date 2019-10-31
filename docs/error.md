Function | UseErr | ErrType | FlushStdout | Terminate
---|:---:|:---:|:---:|:---:
err_msg     | • | errno | • |
err_exit    | • | errno | • | • 
_err_exit   | • | errno |   | • 
err_exit_en | • | errnum| • | • 
fatal       |   |       | • | • 

```
#include <stdio.h>

int printf(const char *restrict format, ...);
int fprintf(FILE *restrict fp, const char *restrict format, ...);
int dprintf(int fd, const char *restrict format, ...);
        // All three return:
        // number of characters output if OK,
        // negative value if output error.
int sprintf(char *restrict buf, const char *restrict format, ...)  ;
        // Returns:
        // number of characters stored in array if OK,
        // negative value if encoding error.
int snprintf(char *restrict buf, size_t n, const char *restrictformat, ...);
        // Returns:
        // number of characters that would have been stored in array if buffer was large enough,
        // negative value if encoding error.
```
The following five variants of the printf family are similar to the previous five, but the variable argument list (...) is replaced with arg.
```
#include <stdarg.h>
#include <stdio.h>

int vprintf(const char *restrict format, va_list arg);
int vfprintf(FILE *restrict fp, const char *restrict format, va_list arg);
int vdprintf(int fd, const char *restrict format, va_list arg);
        // All three return:
        // number of characters output if OK,
        // negative value if output error.
int vsprintf(char *restrict buf, const char *restrict format, va_list arg);
        // Returns:
        // number of characters stored in array if OK,
        // negative value if encoding error.
int vsnprintf(char *restrict buf, size_t n, const char *restrict format, va_list arg);
        // Returns:
        // number of characters that would have been stored in array if buffer was large enough,
        // negative value if encoding error.
```

### TODO
1. The difference between exit() and _exit() - Chapter 25
2. Where to use err_exit_en() and _err_exit()?
3. Dump core?

5. Generate emane.c.inc
6. fflush? / line-buffered?
7. Why save errno in err_msg()?