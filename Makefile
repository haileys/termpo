CFLAGS = -Wall -Wextra -pedantic -Werror -std=c99 -D_DEFAULT_SOURCE

.PHONY: all clean

all: termpo

clean:
	rm -f termpo

termpo: termpo.c
