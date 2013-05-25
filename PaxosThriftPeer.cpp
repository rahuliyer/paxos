#include "PaxosThriftPeer.h"

#include <sys/socket.h>

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace Paxos;

PaxosThriftPeer::PaxosThriftPeer(std::string host, int port) {
  hostname_ = host;
  port_ = port;
  connectTimeout_ = DEFAULT_CONNECT_TIMEOUT;
  readTimeout_ = DEFAULT_READ_TIMEOUT;
  writeTimeout_ = DEFAULT_WRITE_TIMEOUT;

  initialize();
}

// NOTE: This function is not thread safe. It must be called with the 
// clientLock_ held.
void PaxosThriftPeer::initialize() {
  boost::shared_ptr<TSocket> socket(new TSocket(hostname_, port_));
  socket->open();
  
  socket->setConnTimeout(connectTimeout_);
  socket->setRecvTimeout(readTimeout_);
  socket->setSendTimeout(writeTimeout_);
  
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  client_.reset(new PaxosServiceClient(protocol));
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

void PaxosThriftPeer::sendLearn(const std::string& val) {
  std::lock_guard<std::mutex> g(clientLock_);
  for (int i = 0; i < MAX_RETRIES; ++i) {
    try {
      client_->learn(val);
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
