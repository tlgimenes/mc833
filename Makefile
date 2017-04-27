#CXXFLAGS=-Wall -std=c++11 -O0 -g -ggdb	# Debug
CXXFLAGS=-Wall -std=c++11 -O0 -g -ggdb -DNDEBUG # Release
BIN=server client

all: $(BIN)

clean:
	rm ${BIN}
