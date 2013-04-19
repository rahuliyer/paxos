#ifndef __PAXOS_PEER_H__
#define __PAXOS_PEER_H__
#include "Paxos_types.h"

class PaxosPeer {
public:
	virtual void initialize() {}
  virtual void sendPropose(const PaxosProposeArgs& p, PaxosProposeResult& r) {}
  virtual void sendAccept(const PaxosAcceptArgs& a, PaxosAcceptResult& r) {}
};
#endif
