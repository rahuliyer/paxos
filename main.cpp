#include "PaxosThriftPeer.h"
#include "PaxosThriftServer.h"

#include <vector>
#include <thread>
#include <iostream>

using namespace std;

int main(int args, char** argv) {
	std::vector<PaxosPeer> peers;
	PaxosThriftPeer p("localhost", 9090);
	peers.push_back(p);

	PaxosBrain brain(peers);
	int port = atoi(argv[1]);
	PaxosThriftServer server(brain, port);

	cout << "Starting server on port " << port << endl;
	server.start();	

	cout << "Back in main thread!!" << endl;

  return 0;
}

