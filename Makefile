CC = clang
CFLAGS = -Wall -g

all: server

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f server client
