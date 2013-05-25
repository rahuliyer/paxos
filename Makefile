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

THRIFT_OBJS=thrift/gen-cpp/PaxosService.o thrift/gen-cpp/Paxos_types.o \
	thrift/gen-cpp/Paxos_constants.o
OBJS=$(THRIFT_OBJS) PaxosThriftServer.o PaxosServiceHandler.o \
	PaxosBrain.o PaxosFileLogger.o PaxosState.o PaxosThriftPeer.o PaxosClient.o 

all: libpaxos.a PaxosTestServer PaxosTestClient PaxosFileLogReader

PaxosTestClient: libpaxos.a PaxosTestClient.o
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) \
    -o PaxosTestClient PaxosTestClient.o $(PAXOS_LIB) $(SERVER_LIBS)

PaxosTestServer: libpaxos.a PaxosTestServer.o
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) \
    -o PaxosTestServer PaxosTestServer.o $(PAXOS_LIB) $(SERVER_LIBS)

PaxosFileLogReader: PaxosFileLogReader.o
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) $(LIBRARY_INCLUDES) \
    -o PaxosFileLogReader PaxosFileLogReader.o $(PAXOS_LIB) $(SERVER_LIBS)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(FLAGS) $(DEFINES) -c $< -o $@

libpaxos.a: $(OBJS)
	$(AR) rcs libpaxos.a $(OBJS)

.PHONY: runtests
runtests: libpaxos.a
	make -C ./tests clean
	make -C ./tests runtests

.PHONY: thrift
thrift:
	$(THRIFT) --gen cpp -o $(THRIFTDIR) $(THRIFTDIR)/Paxos.thrift
	
.PHONY : clean
clean:
	rm -rf $(THRIFTDIR)/gen-cpp
	rm -f *.o main libpaxos.a
