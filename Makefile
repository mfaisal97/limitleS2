
all: limitless

limitless: 
	g++ -std=c++11 -o limitless main.cpp

clean:
	rm -f limitless
