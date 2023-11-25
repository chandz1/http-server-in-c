CC = clang
CFLAGS = -Wall -g
OBJS = response.o queue-ll.o server.o

all: $(OBJS) server

server: server.o response.o queue-ll.o threads.o
	$(CC) $(CFLAGS) $^ -o $@

response.o: response.c response.h
	$(CC) $(CFLAGS) -c $< -o $@

queue-ll.o: queue-ll.c queue-ll.h
	$(CC) $(CFLAGS) -c $< -o $@

threads.o: threads.c threads.h
	$(CC) $(CFLAGS) -c $< -o $@

server.o: server.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f server *.o
