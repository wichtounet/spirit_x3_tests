default: problem_1

.PHONY: clean

CC=clang++
LD=clang++

BOOST_PREFIX=$(HOME)/build/modular-boost/

CXX_FLAGS=-Iinclude -std=c++11 -stdlib=libc++ $(WARNING_FLAGS) -isystem $(BOOST_PREFIX)/include -g
LD_FLAGS=$(CXX_FLAGS)

problem_1: src/problem_1.cpp
	$(CC) $(CXX_FLAGS) -o problem_1.o -c src/problem_1.cpp
	$(LD) $(LD_FLAGS) -o problem_1 problem_1.o

clean:
	rm -rf *.o
	rm -rf problem_1
