CC = gcc -std=gnu99

CFLAGS = -g -O0
LIBS = -pthread

PROGS = vee

all:$(PROGS)

vee:error.o log.o epoll.o priority_queue.o		\
	timer.o http_request.o http_parse.o rio.o	\
	http.o sock.o threadpool.o util.o vee.o
	$(CC) $(CFLAGS) $? -o vee $(LIBS)

vee.o:vee.c
	$(CC) $(CFLAGS) -c $?

error.o:error.c
	$(CC) $(CFLAGS) -c $?

log.o:log.c
	$(CC) $(CFLAGS) -c $?

epoll.o:epoll.c
	$(CC) $(CFLAGS) -c $?

priority_queue.o:priority_queue.c
	$(CC) $(CFLAGS) -c $?

timer.o:timer.c
	$(CC) $(CFLAGS) -c $?

rio.o:rio.c
	$(CC) $(CFLAGS) -c $?

http_request.o:http_request.c
	$(CC) $(CFLAGS) -c $?

http_parse.o:http_parse.c
	$(CC) $(CFLAGS) -c $?

util.o:util.c
	$(CC) $(CFLAGS) -c $?

http.o:http.c
	$(CC) $(CFLAGS) -c $?

sock.o:sock.c
	$(CC) $(CFLAGS) -c $?

threadpool.o:threadpool.c
	$(CC) $(CFLAGS) -c $?

.PHONY: all clean

clean:
	rm *.o
	rm $(PROGS)
