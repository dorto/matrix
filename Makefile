CC = g++
FLAGS = -std=c++11 -g -Wall 

matris: matris.cpp
		$(CC) $(FLAGS) -o matris matris.cpp
