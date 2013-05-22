#include "PaxosServiceHandler.h"

using namespace Paxos;

PaxosServiceHandler::PaxosServiceHandler(PaxosBrain& brain) : brain_(brain) {
}

void PaxosServiceHandler::propose(PaxosProposeResult& res, const PaxosProposeArgs& args) {
  res = brain_.recvPropose(args);
}

void PaxosServiceHandler::accept(PaxosAcceptResult& res, const PaxosAcceptArgs& args) {
  res = brain_.recvAccept(args);
	brain_.sentAcceptResponse();
}

void PaxosServiceHandler::learn(const std::string& value) {
  brain_.learn(value);
}

int64_t PaxosServiceHandler::getHighestProposalSeen() {
  return brain_.getHighestProposalSeen();
}

