#ifndef __PAXOS_THRIFT_PEER_H__
#define __PAXOS_THRIFT_PEER_H__

#include "PaxosPeer.h"
#include "PaxosService.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#include <mutex>

namespace Paxos {
class PaxosThriftPeer : public PaxosPeer {
public:
	PaxosThriftPeer(std::string hostname, int port);
	~PaxosThriftPeer();
	void initialize();
	virtual void sendPropose(const PaxosProposeArgs& args, PaxosProposeResult& res);
	virtual void sendAccept(const PaxosAcceptArgs& args, PaxosAcceptResult& res);
  virtual void sendLearn(const std::string& val);
  virtual int64_t getHighestProposalSeen();

	void setConnectTimeout(int timeoutInMs);
	void setSendTimeout(int timeoutInMs);
	void setReceiveTimeout(int timeoutInMs);

private:
	std::mutex clientLock_;
	std::shared_ptr<PaxosServiceClient> client_;
	std::string hostname_;
	int port_;
	int connectTimeout_;
	int readTimeout_;
	int writeTimeout_;

	const int MAX_RETRIES = 3;
	const int DEFAULT_CONNECT_TIMEOUT = 100;
	const int DEFAULT_READ_TIMEOUT = 100;
	const int DEFAULT_WRITE_TIMEOUT = 100;
};
}

#endif
