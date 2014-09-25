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

problem_6: src/problem_6.cpp
	$(CC) $(CXX_FLAGS) -o problem_6.o -c src/problem_6.cpp
	$(LD) $(LD_FLAGS) -o problem_6 problem_6.o

problem_7: src/problem_7.cpp
	$(CC) $(CXX_FLAGS) -o problem_7.o -c src/problem_7.cpp
	$(LD) $(LD_FLAGS) -o problem_7 problem_7.o

problem_8: src/problem_8.cpp
	$(CC) $(CXX_FLAGS) -o problem_8.o -c src/problem_8.cpp
	$(LD) $(LD_FLAGS) -o problem_8 problem_8.o

not_nice_1: src/not_nice_1.cpp
	$(CC) $(CXX_FLAGS) -o not_nice_1.o -c src/not_nice_1.cpp
	$(LD) $(LD_FLAGS) -o not_nice_1 not_nice_1.o

monster: src/monster.cpp
	$(CC) -fno-rtti -O2 -ftemplate-depth-2048 $(CXX_FLAGS) -o monster.o -c src/monster.cpp
	$(LD) $(LD_FLAGS) -o monster monster.o

clean:
	rm -rf *.o
	rm -rf problem_1
	rm -rf problem_2
	rm -rf problem_3
	rm -rf problem_4
	rm -rf problem_5
	rm -rf problem_6
	rm -rf problem_7
	rm -rf problem_8
	rm -rf not_nice_1
	rm -rf monster
