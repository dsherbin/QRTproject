PARAMS= -std=c++11 -Wall

p4_client: main.o SRI_CLI.o Rule.o Fact.o QueryParam.o Thread.o QueryThread.o QueryThreadManager.o TCPSocket.o TCPServerSocket.o
	g++ main.o SRI_CLI.o Rule.o Fact.o QueryParam.o Thread.o QueryThread.o QueryThreadManager.o TCPSocket.o TCPServerSocket.o -o p4_client

main.o: main.cpp main.h
	g++ -c main.cpp $(PARAMS) -o main.o

SRI_CLI.o: SRI_CLI.cpp SRI_CLI.h
	g++ -c SRI_CLI.cpp $(PARAMS) -o SRI_CLI.o

Rule.o: Rule.cpp Rule.h
	g++ -c Rule.cpp $(PARAMS) -o Rule.o

Fact.o: Fact.cpp Fact.h
	g++ -c Fact.cpp $(PARAMS) -o Fact.o

QueryParam.o: QueryParam.cpp QueryParam.h
	g++ -c QueryParam.cpp $(PARAMS) -o QueryParam.o

Thread.o: Thread.cpp Thread.h
	g++ -c Thread.cpp $(PARAMS) -o Thread.o

QueryThread.o: QueryThread.cpp QueryThread.h
	g++ -c QueryThread.cpp $(PARAMS) -o QueryThread.o

QueryThreadManager.o: QueryThreadManager.cpp QueryThreadManager.h
	g++ -c QueryThreadManager.cpp $(PARAMS) -o QueryThreadManager.o

TCPSocket.o: TCPSocket.cpp TCPSocket.h
	g++ -c TCPSocket.cpp $(PARAMS) -o TCPSocket.o

TCPServerSocket.o: TCPServerSocket.cpp TCPServerSocket.h
	g++ -c TCPServerSocket.cpp $(PARAMS) -o TCPServerSocket.o

clean:
	rm -f *.o p4_client

