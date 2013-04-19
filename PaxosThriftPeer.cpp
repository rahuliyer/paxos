#include "PaxosThriftPeer.h"

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

PaxosThriftPeer::PaxosThriftPeer(std::string host, int port) {
	hostname_ = host;
	port_ = port;
}

void PaxosThriftPeer::initialize() {
	boost::shared_ptr<TSocket> socket(new TSocket(hostname_, port_));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	std::lock_guard<std::mutex> g(clientLock_);
	client_.reset(new PaxosServiceClient(protocol));
	transport->open();
}

PaxosThriftPeer::~PaxosThriftPeer() {
	client_->getInputProtocol()->getTransport()->close();
}


void PaxosThriftPeer::sendPropose(const PaxosProposeArgs& args, PaxosProposeResult& res) {
	std::lock_guard<std::mutex> g(clientLock_);
	client_->propose(res, args);
}

void PaxosThriftPeer::sendAccept(const PaxosAcceptArgs& args, PaxosAcceptResult& res) {
	std::lock_guard<std::mutex> g(clientLock_);
	client_->accept(res, args);
}

