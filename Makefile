CXX=g++
AR=ar
THRIFT=/usr/local/bin/thrift
INCLUDES=-I. -I./thrift/gen-cpp -I/usr/local/include/thrift
FLAGS=-Wall -Werror -g -std=gnu++11
DEFINES=-DHAVE_CONFIG_H
LIBRARY_INCLUDES=-L/usr/local/lib -L.
THRIFTDIR=./thrift

PAXOS_LIBNAME=libpaxos.a

COMMON_LIBS=-lthrift
SERVER_LIBS=$(COMMON_LIBS) -lthriftnb -levent
PAXOS_LIB=-lpaxos

THRIFT_OBJS=PaxosThriftServer.o PaxosServiceHandler.o thrift/gen-cpp/PaxosService.o thrift/gen-cpp/Paxos_types.o thrift/gen-cpp/Paxos_constants.o
OBJS=$(THRIFT_OBJS) PaxosBrain.o PaxosState.o PaxosThriftPeer.o PaxosClient.o

all: main.cpp paxos_lib
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) -o main main.cpp $(PAXOS_LIB) $(SERVER_LIBS)

%.o: %.cpp thrift
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c $< -o $@

.PHONY : paxos_lib
paxos_lib: $(OBJS)
	$(AR) rcs libpaxos.a $(OBJS)

.PHONY : thrift
thrift: 
	$(THRIFT) --gen cpp -o $(THRIFTDIR) $(THRIFTDIR)/Paxos.thrift

.PHONY : clean
clean:
	rm -rf $(THRIFTDIR)/gen-cpp
	rm -f *.o
