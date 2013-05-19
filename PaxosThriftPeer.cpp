#include "PaxosThriftPeer.h"

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace Paxos;

PaxosThriftPeer::PaxosThriftPeer(std::string host, int port) {
	hostname_ = host;
	port_ = port;

	boost::shared_ptr<TSocket> socket(new TSocket(hostname_, port_));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	std::lock_guard<std::mutex> g(clientLock_);
	client_.reset(new PaxosServiceClient(protocol));
}

void PaxosThriftPeer::initialize() {
	client_->getInputProtocol()->getTransport()->open();
}

PaxosThriftPeer::~PaxosThriftPeer() {
	client_->getInputProtocol()->getTransport()->close();
}

void PaxosThriftPeer::sendPropose(const PaxosProposeArgs& args, PaxosProposeResult& res) {
	std::lock_guard<std::mutex> g(clientLock_);
	
	for (int i = 0; i < MAX_RETRIES; ++i) {
		try {
			client_->propose(res, args);
		} catch (...) {
			if (i == MAX_RETRIES - 1) {
				throw;
			} else {
				initialize();
			}
      continue;
		}
    break;
	}
}

void PaxosThriftPeer::sendAccept(const PaxosAcceptArgs& args, PaxosAcceptResult& res) {
	std::lock_guard<std::mutex> g(clientLock_);
	for (int i = 0; i < MAX_RETRIES; ++i) {
		try {
			client_->accept(res, args);
		} catch (...) {
			if (i == MAX_RETRIES - 1) {
				throw;
			} else {
				initialize();
			}
      continue;
		}
    break;
	}
}

int64_t PaxosThriftPeer::getHighestProposalSeen() {
  std::lock_guard<std::mutex> g(clientLock_);
  return client_->getHighestProposalSeen();
}
