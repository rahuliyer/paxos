#ifndef __PAXOS_PEER_H__
#define __PAXOS_PEER_H__
#include "Paxos_types.h"
#include <iostream>

namespace Paxos {
class PaxosPeer {
public:
	virtual void initialize() {}
  virtual void sendPropose(const PaxosProposeArgs& p, PaxosProposeResult& r) = 0; 
  virtual void sendAccept(const PaxosAcceptArgs& a, PaxosAcceptResult& r) = 0;
  virtual int64_t getHighestProposalSeen() = 0;
};
}
#endif
