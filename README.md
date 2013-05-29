Paxos
=====

A simple Paxos implementation in C++. For more information on the Paxos protocol, go here: 
http://en.wikipedia.org/wiki/Paxos_%28computer_science%29

The key classes in this library are:

PaxosBrain
----------
This class implements the core of the Paxos protocol. It plays the role of the acceptor in the protocol description.

PaxosPeer
---------
This interface represents a connection to a replica in the Paxos quorum. PaxosThriftClient is an implementation of this interface in thrift.

PaxosClient
-----------
This class implements the logic required to reach consensus. It implements the roles of the client and the proposer.

PaxosLearner
------------
This interface implements the role of the learner. Typically an application will implement this interface to "learn" of events that have been agreed upon.
