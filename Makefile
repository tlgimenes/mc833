#CXXFLAGS=-Wall -O0 -g -ggdb	# Debug
CXXFLAGS=-Wall -O0 -g -ggdb -DNDEBUG # Release
BIN=server client

all: $(BIN)

clean:
	rm ${BIN}
