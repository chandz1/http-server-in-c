CC = clang
CFLAGS = -Wall -g

all: server client

%: %.c
	$(CC) $(CFLAGS) $< -o $@
