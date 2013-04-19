#include "PaxosThriftServer.h"
#include "PaxosServiceHandler.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

PaxosThriftServer::PaxosThriftServer(PaxosBrain& brain, int port) : brain_(brain), port_(port) {
}

void PaxosThriftServer::start() {
	boost::shared_ptr<TProtocolFactory> protocolFactory(
		new TBinaryProtocolFactory());
	boost::shared_ptr<PaxosServiceHandler>	handler(new PaxosServiceHandler(brain_));
	boost::shared_ptr<TProcessor> processor(new PaxosServiceProcessor(handler));

	TNonblockingServer server(processor, protocolFactory, port_);
	serverThread_ = std::thread(std::bind(&TNonblockingServer::serve, server));
}

PaxosThriftServer::~PaxosThriftServer() {
	serverThread_.join();
}

