CC=g++
#CXXFLAGS=-O3 -DNDEBUG -Wall
CXXFLAGS=-O0 -Wall -g 
DEP=car.o client.o client_car.o predictor.o
BIN=test_predictor

all: $(DEP)
	$(CC) $(CXXFLAGS) -I../include $(DEP) $(BIN).cpp -o $(BIN)

%.o: ../src/%.cpp
	$(CC) -c $(CXXFLAGS) -I../include $< -o $@

clear:
	rm $(BIN) $(DEP)
