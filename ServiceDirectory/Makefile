
all: limitless

limitless:
	g++ -std=c++11 -o limitless  -ggdb main.cpp ../source/*.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`


clean:
	rm -f limitless
