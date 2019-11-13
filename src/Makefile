CC = gcc -std=gnu99

CFLAGS = -g -O0
LIBS = -pthread

PROGS = vee

all:$(PROGS)

vee:error.o log.o epoll.o priority_queue.o timer.o request.o http.o sock.o threadpool.o vee.o
	$(CC) $(CFLAGS) $? -o vee $(LIBS)

vee_:vee_.o error.o epoll.o
	$(CC) $(CFLAGS) $? -o vee_

vee.o:vee.c
	$(CC) $(CFLAGS) -c $?

vee_.o:vee_.c
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

request.o:request.c
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
