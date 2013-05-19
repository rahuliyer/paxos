#ifndef __PAXOS_THRIFT_SERVER_H__
#define __PAXOS_THRIFT_SERVER_H__

#include "PaxosBrain.h"

#include <thread>

namespace Paxos {
class PaxosThriftServer {
public:
	PaxosThriftServer(PaxosBrain&, int port = 9090);
	~PaxosThriftServer();
	void start();

private:
	std::thread	serverThread_;
	PaxosBrain& brain_;
	int port_;
};
}

#endif
