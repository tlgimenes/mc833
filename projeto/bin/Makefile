CC=g++
#CXXFLAGS=-O3 -DNDEBUG -Wall
CXXFLAGS=-O0 -Wall -g 
DEP=car.o server.o utils.o
BIN=server_main

all: $(DEP)
	$(CC) $(CXXFLAGS) -I../include $(DEP) ../src/$(BIN).cpp -o $(BIN)

%.o: ../src/%.cpp
	$(CC) -c $(CXXFLAGS) -I../include $< -o $@

clear:
	rm $(BIN) $(DEP)
