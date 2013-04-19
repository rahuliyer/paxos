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
	PaxosThriftServer server(brain, 9090);
	server.start();	

	cout << "Back in main thread!!" << endl;

  return 0;
}

