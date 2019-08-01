#include <strings.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "http.h"
#include "http_parse.h"
#include "http_request.h"
#include "epoll.h"
#include "error.h"
#include "timer.h"

static const char *get_file_type(const char *type);
static void parse_uri(char *uri, int length, char *filename, char *querystring);
static void do_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
static void serve_static(int fd, char *filename, size_t filesize, vee_http_out_t *out);
static char *ROOT = NULL;

mime_type_t vee_mime[] = 
{
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/msword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".css", "text/css"},
    {NULL ,"text/plain"}
};

void do_request(void *ptr) {
    vee_http_request_t *r = (vee_http_request_t *)ptr;
    int fd = r->fd;
    int rc, n;
    char filename[SHORTLINE];
    struct stat sbuf;
    ROOT = r->root;
    char *plast = NULL;
    size_t remain_size;

    vee_del_timer(r);
    for (;;) {
        plast = &r->buf[r->last % MAX_BUF];
        remain_size = MIN(MAX_BUF - (r->last - r->pos) - 1, MAX_BUF - r->last % MAX_BUF);

        n = read(fd, plast, remain_size);
        check(r->last - r->pos < MAX_BUF, "request buffer overflow!");

        if (n == 0) {
            // EOF
            log_info("read return 0, ready to close fd %d, remain_size = %zu", fd, remain_size);
            goto err;
        }

        if (n < 0) {
            if (errno != EAGAIN) {
                log_err("read err, and errno = %d", errno);
                goto err;
            }
            break;
        }

        r->last += n;
        check(r->last - r->pos < MAX_BUF, "request buffer overflow!");

        log_info("ready to parse request line");

        rc = vee_http_parse_request_line(r);
        if (rc == VEE_AGAIN) {
            continue;
        } else if (rc != VEE_OK) {
            log_err("rc != VEE_OK");
            goto err;
        }

        log_info("method == %.*s", (int)(r->method_end - r->request_start), (char *)r->request_start);
        log_info("uri == %.*s", (int)(r->uri_end - r->uri_start), (char *)r->uri_start);

        debug("ready to parse request body");

        rc = vee_http_parse_request_body(r);
        if (rc == VEE_AGAIN) {
            continue;
        } else if (rc != VEE_OK) {
            log_err("rc != VEE_OK");
            goto err;
        }

        vee_http_out_t *out = (vee_http_out_t *)malloc(sizeof(vee_http_out_t));
        if (out == NULL) {
            log_err("no enough space for vee_http_out_t");
            exit(1);
        }

        rc = vee_init_out_t(out, fd);
        check(rc == VEE_OK, "vee_init_out_t");

        parse_uri(r->uri_start, r->uri_end - r->uri_start, filename, NULL);

        if(stat(filename, &sbuf) < 0) {
            do_error(fd, filename, "404", "Not Found", "vee can't find the file");
            continue;
        }

        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            do_error(fd, filename, "403", "Forbidden", "vee can't read the file");
            continue;
        }

        out->mtime = sbuf.st_mtime;

        vee_http_handle_header(r, out);
        check(list_empty(&(r->list)) == 1, "header list should be empty");

        if (out->status == 0) {
            out->status = VEE_HTTP_OK;
        }

        serve_static(fd, filename, sbuf.st_size, out);

        if (!out->keep_alive) {
            log_info("no keep_alive! ready to close");
            free(out);
            goto close;
        }
        free(out);

    }

    struct epoll_event event;
    event.data.ptr = ptr;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;    // why oneshot?

    vee_epoll_mod(r->epfd, r->fd, &event);
    vee_add_timer(r, TIMEOUT_DEFAULT, vee_http_close_conn);

    return;

err:
close:
    rc = vee_http_close_conn(r);
    check(rc == 0, "do_request: vee_http_close_conn");
}

static void parse_uri(char *uri, int uri_length, char *filename, char *querystring) {
    check(uri != NULL, "parse_uri: uri is NULL");
    uri[uri_length] = '\0';

    char *question_mark = strchr(uri, '?');
    int file_length;
    if (question_mark) {
        file_length = (int)(question_mark - uri);
        debug("file_length = (question_mark - uri) = %d", file_length);
    } else {
        file_length = uri_length;
        debug("file_length = uri_length = %d", file_length);
    }

    if (querystring) {
        // TODO
    }

    strcpy(filename, ROOT); // what's ROOT meaning?

    if (uri_length > (SHORTLINE >> 1)) {
        log_err("uri too long: %.*s", uri_length, uri);
        return;
    }

    debug("before strncat, filename = %s, uri = %.*s, file_len = %d", filename, file_length, uri, file_length);
    strncat(filename, uri, file_length);

    // strrchr ?
    char *last_comp = strrchr(filename, '/');
    char *last_dot = strrchr(filename, '.');
    if (last_dot == NULL && filename[strlen(filename)-1] != '/') {
        strcat(filename, "/");
    }

    if (filename[strlen(filename)-1] == '/') {
        strcat(filename, "index.html");
    }

    log_info("filename = %s", filename);

    return;
}

static void do_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
    char header[MAXLINE], body[MAXLINE];

    sprintf(body, "<html><title>Vee Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\n", body);
    sprintf(body, "%s%s: %s\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\n</p>", body, longmsg, cause);
    sprintf(body, "%s<hr><em>Vee web server</em>\n</body></html>", body);

    sprintf(header, "HTTP/1.1 %s %s\r\n", errnum, shortmsg);
    sprintf(header, "%sServer: Vee\r\n", header);
    sprintf(header, "%sContent-type: text/html\r\n", header);
    sprintf(header, "%sConnection: close\r\n", header);
    sprintf(header, "%sContent-length: %d\r\n\r\n", header, (int)strlen(body));

    rio_writen(fd, header, strlen(header));
    rio_writen(fd, body, strlen(body));

    return;
}

static void serve_static(int fd, char *filename, size_t filesize, vee_http_out_t *out) {
    char header[MAXLINE];
    char buf[SHORTLINE];
    size_t n;
    struct tm tm;

    const char *file_type;
    const char *dot_pos = strrchr(filename, '.');
    file_type = get_file_type(dot_pos);

    sprintf(header, "HTTP/1.1 %d %s\r\n", out->status, get_shortmsg_from_status_code(out->status));

    if (out->keep_alive) {
        sprintf(header, "%sConnection: keep-alive\r\n", header);
        sprintf(header, "%sKeep-Alive: timeout=%d\r\n", header, TIMEOUT_DEFAULT);
    }

    if (out->modified) {
        sprintf(header, "%sContent-type: %s\r\n", header, file_type);
        sprintf(header, "%sContent-length: %zu\r\n", header, filesize);
        localtime_r(&(out->mtime), &tm);
        strftime(buf, SHORTLINE, "%a, %d %b %Y %H:%M:%S GMT", &tm);
        sprintf(header, "%sLast-Modified: %s\r\n", header, buf);
    }

    sprintf(header, "%sServer: Vee\r\n", header);
    sprintf(header, "%s\r\n", header);

    n = (size_t)rio_writen(fd, header, strlen(header));
    check(n == strlen(header), "rio_writen error, errno = %d", errno);
    if (n != strlen(header)) {
        log_err("n != strlen(header)");
        goto out;
    }

    if (!out->modified) {
        goto out;
    }

    int srcfd = open(filename, O_RDONLY, 0);
    check(srcfd > 2, "open error");

    // what's mmap and munmap meaning?
    char *srcaddr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    check(srcaddr != (void *) -1, "mmap error");
    close(srcfd);

    n = rio_writen(fd, srcaddr, filesize);

    munmap(srcaddr, filesize);

out:
    return;
}

static const char *get_file_type(const char *type) {
    if (type == NULL) {
        return "text/plain";
    }

    int i;
    for (i = 0; vee_mime[i].type != NULL; ++i) {
        if (strcmp(type, vee_mime[i].type) == 0)
            return vee_mime[i].value;
    }

    return vee_mime[i].value;
}
