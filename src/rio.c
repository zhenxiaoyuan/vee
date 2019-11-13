#include "rio.h"

ssize_t rio_readn(int fd, char *ptr, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return -1;          /* error, return -1 */
            else
                break;              /* error, return amount read so far */
        } else if (nread == 0)      /* end-of-file */
            break;

        nleft -= nread;
        ptr += nread;
    }

    return (n - nleft);             /* return >= 0 */
}

ssize_t rio_writen(int fd, const char *ptr, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return -1;          /* error, return -1 */
            else
                break;              /* error, return amount written so far */
        } else if (nwritten == 0)
            break;

        nleft -= nwritten;
        ptr += nwritten;
    }

    return (n - nleft);             /* return >= 0 */
}
