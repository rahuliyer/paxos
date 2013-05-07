CXX=g++
DEFINES=-DHAVE_CONFIG_H
FLAGS=-Wall -Werror -g -std=gnu++11
PAXOS_LIB=-lpaxos
PAXOS_LIB_PATH=-L..

GTEST_DIR=/home/rni/gmock-svn/gtest
GMOCK_DIR=/home/rni/gmock-svn/
GMOCK_INCLUDES=-isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include
GMOCK_LIB_DIR=-L$(GMOCK_DIR)
GMOCK_LIB=-lgmock 
GMOCK_DEP_LIBS=-pthread

INCLUDES=-I.. -I ../thrift/gen-cpp -I. -I/usr/local/include/thrift

PaxosTest: PaxosTest.cpp
	$(CXX) $(FLAGS) $(DEFINES) $(INCLUDES) $(PAXOS_LIB_PATH) $(GMOCK_INCLUDES) $(GMOCK_LIB_DIR) $(GMOCK_DEP_LIBS) PaxosTest.cpp $(GMOCK_LIB) -o PaxosTest $(PAXOS_LIB)

