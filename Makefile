
all: limitless

limitless: 
	g++ -o limitless main.cpp

clean:
	rm -f limitless
