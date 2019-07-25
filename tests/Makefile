CC = gcc

CFLAGS = -g -O0

PROGS = client

all: $(PROGS)

yuan: client.o
	$(CC) $(CFLAGS) -o $@ $?

.PHONY: all clean

clean:
	rm *.o
	rm $(PROGS)
