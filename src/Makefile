CC = gcc

CFLAGS = -g -O0

PROGS = yuan

all: $(PROGS)

yuan: yuan.o
	$(CC) $(CFLAGS) -o $@ $?

.PHONY: all clean

clean:
	rm *.o
	rm $(PROGS)
