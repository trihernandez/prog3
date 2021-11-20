# CC=gcc
# CFLAGS=-I.
CXX=clang++

prog3: prog3.o
	${CXX} prog3.o -o prog3

prog3.o: prog3.cpp
	${CXX} -c prog3.cpp -o prog3.o

numbergen: numbergen.o
	${CXX} numbergen.o -o numbergen

numbergen.o: numbergen.cpp
	${CXX} -c numbergen.cpp -o numbergen.o