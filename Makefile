CC = clang
CFLAGS = -Wall -g
OBJS = response.o server.o

all: $(OBJS) server

server: server.o response.o
	$(CC) $(CFLAGS) $^ -o $@

response.o: response.c response.h
	$(CC) $(CFLAGS) -c $< -o $@

server.o: server.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f server *.o
