CC=g++

main: main.cpp
	g++ -O3 -o main main.cpp -std=c++0x -pthread
