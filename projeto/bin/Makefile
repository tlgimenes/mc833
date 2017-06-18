CC=g++
#CXXFLAGS=-O3 -DNDEBUG -Wall
CXXFLAGS=-O0 -Wall -g
SDEP=car.o server.o tcp_server.o udp_server.o utils.o predictor.o
SBIN=server_main
CDEP=car.o client_car.o client.o tcp_client.o udp_client.o utils.o
CBIN=client_main

all: $(SBIN) $(CBIN)

$(SBIN): ../src/$(SBIN).cpp $(SDEP)
	$(CC) $(CXXFLAGS) -I../include $(SDEP) ../src/$(SBIN).cpp -o $(SBIN)

$(CBIN): ../src/$(CBIN).cpp $(CDEP)
	$(CC) $(CXXFLAGS) -I../include $(CDEP) ../src/$(CBIN).cpp -o $(CBIN)

%.o: ../src/%.cpp
	$(CC) -c $(CXXFLAGS) -I../include $< -o $@

clear:
	rm $(SBIN) $(SDEP) $(CBIN) $(CDEP) -f
