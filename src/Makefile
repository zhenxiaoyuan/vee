CC = gcc -std=gnu99

CFLAGS = -g -O0

PROGS = vee vee_

#all:vee.o error.o epoll.o
#	$(CC) $(CFLAGS) $? -o $(PROGS)

all:$(PROGS)

vee:vee.o error.o epoll.o
	$(CC) $(CFLAGS) $? -o vee

vee_:vee_.o error.o epoll.o
	$(CC) $(CFLAGS) $? -o vee_

vee.o:vee.c
	$(CC) $(CFLAGS) -c $?

vee_.o:vee_.c
	$(CC) $(CFLAGS) -c $?

error.o:error.c
	$(CC) $(CFLAGS) -c $?

epoll.o:epoll.c
	$(CC) $(CFLAGS) -c $?

.PHONY: all clean

clean:
	rm *.o
	rm $(PROGS)
