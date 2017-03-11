PARAMS= -std=c++11 -Wall

p3: main.o SRI_CLI.o SRI_Engine.o Rule.o Fact.o QueryParam.o Thread.o QueryThread.o QueryThreadManager.o
	g++ main.o SRI_CLI.o SRI_Engine.o Rule.o Fact.o QueryParam.o Thread.o QueryThread.o QueryThreadManager.o -o p3

main.o: main.cpp main.h
	g++ -c main.cpp $(PARAMS) -o main.o

SRI_CLI.o: SRI_CLI.cpp SRI_CLI.h
	g++ -c SRI_CLI.cpp $(PARAMS) -o SRI_CLI.o

SRI_Engine.o: SRI_Engine.cpp SRI_Engine.h
	g++ -c SRI_Engine.cpp $(PARAMS) -o SRI_Engine.o

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

clean:
	rm -f *.o p3

