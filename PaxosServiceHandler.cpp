#include "PaxosServiceHandler.h"

PaxosServiceHandler::PaxosServiceHandler(PaxosBrain& brain) : brain_(brain) {
}

void PaxosServiceHandler::propose(PaxosProposeResult& res, const PaxosProposeArgs& args) {
  res = brain_.recvPropose(args);
}

void PaxosServiceHandler::accept(PaxosAcceptResult& res, const PaxosAcceptArgs& args) {
  res = brain_.recvAccept(args);
}

