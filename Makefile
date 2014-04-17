default: problem_1

.PHONY: clean

CC=clang++
LD=clang++

BOOST_PREFIX=$(HOME)/build/modular-boost/

CXX_FLAGS=-Iinclude -std=c++1y -stdlib=libc++ $(WARNING_FLAGS) -isystem $(BOOST_PREFIX)/include
LD_FLAGS=$(CXX_FLAGS) -L $(BOOST_PREFIX)/lib -lboost_program_options
LD_TEST_FLAGS=$(LD_FLAGS) -lboost_unit_test_framework

RELEASE_FLAGS=-g -DLOGGING_DISABLE -DNDEBUG -O3 -flto -march=native -fvectorize -fslp-vectorize-aggressive -fomit-frame-pointer

# Search the source files

problem_1: src/problem_1.cpp
	$(CC) $(CXX_FLAGS) $(RELEASE_FLAGS) -o problem_1.o -c src/problem_1.cpp
	$(LD) $(LD_FLAGS) $(RELEASE_FLAGS) -o problem_1 problem_1.o

clean:
	rm -rf *.o
	rm -rf problem_1
