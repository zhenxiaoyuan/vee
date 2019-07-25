#include <unistd.h>
#include <stdio.h>
#include <string.h>         /* use for str_len */
#include <arpa/inet.h>      /* use for inet_addr */

#define MAX_BUF     256     /* buffer size for read and write */

#define SERV_ADDR   "127.0.0.1"
#define PORT_NUM    7777

int
main(int argc, char *argv[])
{
    int sv_sock;
    struct sockaddr_in sv_addr;
    int str_len;
    char buf[MAX_BUF];

    sv_sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&sv_addr, sizeof(sv_addr));
    sv_addr.sin_family      = AF_INET;
    sv_addr.sin_addr.s_addr = inet_addr(SERV_ADDR);
    sv_addr.sin_port        = htons(PORT_NUM);

    if (connect(sv_sock, (struct sockaddr *) &sv_addr, sizeof(sv_addr)) == -1)
        printf("connect error");

    while (fgets(buf, MAX_BUF, stdin) != NULL) {
        write(sv_sock, buf, strlen(buf));

        if (read(sv_sock, buf, MAX_BUF) == 0)
            printf("server terminated");

        fputs(buf, stdout);
    }
}

