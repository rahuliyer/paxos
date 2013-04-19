#include "PaxosServiceHandler.h"
#include "PaxosPeer.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>

#include <vector>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

int main(int args, char** argv) {
	std::vector<PaxosPeer> peers;
	PaxosBrain brain(peers);
	
	boost::shared_ptr<TProtocolFactory> protocolFactory(
		new TBinaryProtocolFactory());
	boost::shared_ptr<PaxosServiceHandler>	handler(new PaxosServiceHandler(brain));
	boost::shared_ptr<TProcessor> processor(new PaxosServiceProcessor(handler));

	TNonblockingServer server(processor, protocolFactory, 9090);

	server.serve();
  return 0;
}

