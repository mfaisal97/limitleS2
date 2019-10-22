
all: limitless

limitless: 
	gcc -static -o limitless main.cpp source/UDPSocket.cpp source/Message.cpp source/UDPClientSocket.cpp source/UDPServerSocket.cpp  source/Client.cpp  source/Server.cpp

clean:
	rm -f limitless
