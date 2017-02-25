PARAMS= -std=c++11

hw4: main.o SRI_CLI.o SRI_Engine.o
	g++ main.o SRI_CLI.o SRI_Engine.o -o hw4

main.o: main.cpp main.h
	g++ -c main.cpp $(PARAMS) -o main.o

SRI_CLI.o: SRI_CLI.cpp SRI_CLI.h
	g++ -c SRI_CLI.cpp $(PARAMS) -o SRI_CLI.o

SRI_Engine.o: SRI_Engine.cpp SRI_Engine.h
	g++ -c SRI_Engine.cpp $(PARAMS) -o SRI_Engine.o

clean:
	rm -f *.o hw4

