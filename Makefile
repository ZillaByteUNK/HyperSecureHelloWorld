makefile
CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2
LDFLAGS = -lssl -lcrypto

SRC = src/main.c src/config.c src/logger.c src/crypto.c src/thread_manager.c src/verifier.c src/utils.c
OBJ = $(SRC:.c=.o)
INCLUDE = -Iinclude

all: hello_world

hello_world: $(OBJ)
	$(CC) $(CFLAGS) -o hello_world $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f src/*.o hello_world

.PHONY: all clean
