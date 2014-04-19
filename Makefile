default: problem_1

.PHONY: clean

CC=clang++
LD=clang++

BOOST_PREFIX=$(HOME)/build/modular-boost/

CXX_FLAGS=-Iinclude -std=c++11 -stdlib=libc++ $(WARNING_FLAGS) -isystem $(BOOST_PREFIX)/include
LD_FLAGS=$(CXX_FLAGS)

problem_1: src/problem_1.cpp
	$(CC) $(CXX_FLAGS) -o problem_1.o -c src/problem_1.cpp
	$(LD) $(LD_FLAGS) -o problem_1 problem_1.o

problem_2: src/problem_2.cpp
	$(CC) $(CXX_FLAGS) -o problem_2.o -c src/problem_2.cpp
	$(LD) $(LD_FLAGS) -o problem_2 problem_2.o

problem_3: src/problem_3.cpp
	$(CC) $(CXX_FLAGS) -o problem_3.o -c src/problem_3.cpp
	$(LD) $(LD_FLAGS) -o problem_3 problem_3.o

clean:
	rm -rf *.o
	rm -rf problem_1
	rm -rf problem_2
