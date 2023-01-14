CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Wpedantic -Werror
OBJ=dbuffer.o
TEST=test.cpp

ifdef DEBUG
ifeq ($(DEBUG),valgrind)
	CXXFLAGS+=-Og -g
endif
ifeq ($(DEBUG),asan)
	CXXFLAGS+=-Og -fsanitize=address,undefined
endif
else
	CXXFLAGS+=-O2
endif

all: test00

test00: $(OBJ)
	$(CXX) $(CXXFLAGS) $< $(TEST) -o $@

clean:
	rm -f $(OBJ) test00
