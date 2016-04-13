CFLAGS = -Wall -Wextra -pedantic -Werror -std=c99

.PHONY: all clean

all: termpo

clean:
	rm -f termpo

termpo: termpo.c
