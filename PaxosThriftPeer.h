#ifndef __PAXOS_THRIFT_PEER_H__
#define __PAXOS_THRIFT_PEER_H__

#include "PaxosPeer.h"
#include "PaxosService.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

class PaxosThriftPeer : public PaxosPeer {
public:
	PaxosThriftPeer(std::string hostname, int port);
	~PaxosThriftPeer();
	void sendPropose(const PaxosProposeArgs& args, PaxosProposeResult& res);
	void sendAccept(const PaxosAcceptArgs& args, PaxosAcceptResult& res);

private:
	std::shared_ptr<PaxosServiceClient> client_;
};
#endif
