CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -Werror -Wstrict-prototypes
OBJ=dbuffer.o
TEST=test.c

ifdef DEBUG
ifeq ($(DEBUG),valgrind)
	CFLAGS+=-Og -g
endif
ifeq ($(DEBUG),asan)
	CFLAGS+=-Og -fsanitize=address,undefined
endif
else
	CFLAGS+=-O2
endif

all: test00

test00: $(OBJ)
	$(CC) $(CFLAGS) $< $(TEST) -o $@

clean:
	rm -f $(OBJ) test00
