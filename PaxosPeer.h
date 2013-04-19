#ifndef __PAXOS_PEER_H__
#define __PAXOS_PEER_H__
#include "Paxos_types.h"

class PaxosPeer {
public:
  virtual void sendPropose(PaxosProposeArgs& p, PaxosProposeResult& r) {}
  virtual void sendAccept(PaxosAcceptArgs& a, PaxosAcceptResult& r) {}
};
#endif
