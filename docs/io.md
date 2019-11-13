IO

1. open
```c
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags, ... /* mode_t mode */);
    // Returns file descriptor on success, or –1 on error
```

2. read
```c
#include <unistd.h>

ssize_t read(int fd, void *buffer, size_t count);
    //Returns number of bytes read, 0 on EOF, or –1 on error
```
size_t == unsigned int  
ssize_t == signed int  

Because the terminating **null byte** requires a byte of memory, the size of buffer must
be at least one greater than the largest string we expect to read.

3. write
```c
#include <unistd.h>
ssize_t write(int fd, void *buffer, size_t count);
    //Returns number of bytes written, or –1 on error
```

4. close
```c
#include <unistd.h>
int close(int fd);
    //Returns 0 on success, or –1 on error
```
- When a process terminates, all of its open file descriptors are **automatically** closed.
- close() should be **bracketed** with error checking code

------

Futher Details

EAGAIN and EWOULDBLOCK
1. returned by nonblocking calls
2. these two error constants are synonymous

All system calls are executed atomically.

synchronized I/O completion (fsync)
1. synchronized I/O data integrity (fdatasync)
2.  synchronized I/O file integrity completion

the differece between fflush and fsync?

---------

TODO: 
3.1 system call