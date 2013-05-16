CXX=g++
DEFINES=-DHAVE_CONFIG_H
FLAGS=-Wall -Werror -g -std=gnu++11
PAXOS_LIB=-lpaxos -lthrift
PAXOS_LIB_PATH=-L..

GTEST_DIR=/home/rni/gmock-svn/gtest
GMOCK_DIR=/home/rni/gmock-svn/
GMOCK_INCLUDES=-isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
GMOCK_LIB_DIR=-L$(GMOCK_DIR)
GMOCK_LIB=-lgmock 
GMOCK_DEP_LIBS=-pthread

INCLUDES=-I.. -I ../thrift/gen-cpp -I. -I/usr/local/include/thrift

all: PaxosPeerTest PaxosFileLoggerTest

PaxosPeerTest: PaxosPeerTest.cpp
	$(CXX) $(FLAGS) $(DEFINES) $(INCLUDES) $(PAXOS_LIB_PATH) \
    $(GMOCK_INCLUDES) $(GMOCK_LIB_DIR) $(GMOCK_DEP_LIBS) \
    PaxosPeerTest.cpp $(GMOCK_LIB) -o PaxosPeerTest $(PAXOS_LIB)

PaxosFileLoggerTest: PaxosFileLoggerTest.cpp
	$(CXX) $(FLAGS) $(DEFINES) $(INCLUDES) $(PAXOS_LIB_PATH) \
    $(GMOCK_INCLUDES) $(GMOCK_LIB_DIR) $(GMOCK_DEP_LIBS) \
    PaxosFileLoggerTest.cpp $(GMOCK_LIB) -o PaxosFileLoggerTest $(PAXOS_LIB)

.PHONY : clean
clean:
	rm -f *.o PaxosPeerTest PaxosFileLoggerTest

