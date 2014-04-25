default: problem_1

.PHONY: clean

CC=clang++
LD=clang++

BOOST_PREFIX=$(HOME)/build/modular-boost/

CXX_FLAGS=-Iinclude -std=c++1y -stdlib=libc++ $(WARNING_FLAGS) -isystem $(BOOST_PREFIX)/include
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

problem_4: src/problem_4.cpp
	$(CC) $(CXX_FLAGS) -o problem_4.o -c src/problem_4.cpp
	$(LD) $(LD_FLAGS) -o problem_4 problem_4.o

problem_5: src/problem_5.cpp
	$(CC) $(CXX_FLAGS) -o problem_5.o -c src/problem_5.cpp
	$(LD) $(LD_FLAGS) -o problem_5 problem_5.o

clean:
	rm -rf *.o
	rm -rf problem_1
	rm -rf problem_2
	rm -rf problem_3
	rm -rf problem_4
