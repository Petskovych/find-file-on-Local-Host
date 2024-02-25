CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -pthread
CXXFLAGS = -Wall -Wextra -pthread

all: main

file_search_lib.o: file_search_lib.c file_search_lib.h
    $(CC) $(CFLAGS) -c file_search_lib.c -o file_search_lib.o

file_search_lib.a: file_search_lib.o
    ar rcs file_search_lib.a file_search_lib.o

main: main.cpp file_search_lib.a
    $(CXX) $(CXXFLAGS) main.cpp file_search_lib.a -o main

clean:
    rm -f *.o *.a main
