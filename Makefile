CXX=g++
INCLUDES=-I. -I./thrift/gen-cpp -I/usr/local/include/thrift
FLAGS=-Wall -Werror -g -std=gnu++11
DEFINES=-DHAVE_CONFIG_H
LIBRARY_INCLUDES=-L/usr/local/lib

COMMON_LIBS=-lthrift
SERVER_LIBS=$(COMMON_LIBS) -lthriftnb -levent

all: main.cpp PaxosBrain.o PaxosState.o PaxosThriftPeer.o
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) -o main main.cpp PaxosThriftServer.cpp PaxosServiceHandler.cpp thrift/gen-cpp/PaxosService.cpp thrift/gen-cpp/Paxos_types.cpp thrift/gen-cpp/Paxos_constants.cpp PaxosBrain.o PaxosState.o PaxosThriftPeer.o $(SERVER_LIBS)

PaxosThriftPeer.o: PaxosThriftPeer.cpp
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c PaxosThriftPeer.cpp

PaxosBrain.o: PaxosBrain.cpp
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c PaxosBrain.cpp

PaxosState.o: PaxosState.cpp
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c PaxosState.cpp	

clean:
	rm -f *.o
