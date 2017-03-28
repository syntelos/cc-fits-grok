
CC = clang

CFLAGS = -g

LDFLAGS =



main: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

