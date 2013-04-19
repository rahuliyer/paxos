#ifndef __PAXOS_THRIFT_PEER_H__
#define __PAXOS_THRIFT_PEER_H__

#include "PaxosPeer.h"
#include "PaxosService.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#include <mutex>

class PaxosThriftPeer : public PaxosPeer {
public:
	PaxosThriftPeer(std::string hostname, int port);
	~PaxosThriftPeer();
	void initialize();
	void sendPropose(const PaxosProposeArgs& args, PaxosProposeResult& res);
	void sendAccept(const PaxosAcceptArgs& args, PaxosAcceptResult& res);

private:
	std::mutex clientLock_;
	std::shared_ptr<PaxosServiceClient> client_;
	std::string hostname_;
	int port_;
};
#endif
