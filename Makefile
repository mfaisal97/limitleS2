
all: limitless

limitless: 
	g++ -o limitless main.cpp source/UDPSocket.cpp source/Message.cpp source/UDPClientSocket.cpp source/UDPServerSocket.cpp  source/Client.cpp  source/Server.cpp 

clean:
	rm -f limitless
