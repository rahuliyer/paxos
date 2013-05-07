CXX=g++
THRIFT=/usr/local/bin/thrift
INCLUDES=-I. -I./thrift/gen-cpp -I/usr/local/include/thrift
FLAGS=-Wall -Werror -g -std=gnu++11
DEFINES=-DHAVE_CONFIG_H
LIBRARY_INCLUDES=-L/usr/local/lib
THRIFTDIR=./thrift

COMMON_LIBS=-lthrift
SERVER_LIBS=$(COMMON_LIBS) -lthriftnb -levent

all: main.cpp PaxosBrain.o PaxosState.o PaxosThriftPeer.o
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) -o main main.cpp PaxosThriftServer.cpp PaxosServiceHandler.cpp thrift/gen-cpp/PaxosService.cpp thrift/gen-cpp/Paxos_types.cpp thrift/gen-cpp/Paxos_constants.cpp PaxosBrain.o PaxosState.o PaxosThriftPeer.o $(SERVER_LIBS)

%.o: %.cpp thrift
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c $< -o $@

.PHONY : thrift
thrift: 
	$(THRIFT) --gen cpp -o $(THRIFTDIR) $(THRIFTDIR)/Paxos.thrift

.PHONY : clean
clean:
	rm -rf $(THRIFTDIR)/gen-cpp
	rm -f *.o
