#ifndef __PAXOS_BRAIN_H__
#define __PAXOS_BRAIN_H__

#include "Paxos_types.h"
#include "PaxosThriftPeer.h"
#include "PaxosState.h"
#include "PaxosLearner.h"

class PaxosBrain {
private:
  PaxosState                    state_;
	PaxosLearner&						      learner_;

public:
  PaxosBrain(PaxosLearner&);
  PaxosProposeResult  recvPropose(const PaxosProposeArgs&);
  PaxosAcceptResult   recvAccept(const PaxosAcceptArgs&);
  void                sentAcceptResponse();
  int64_t             getHighestProposalSeen();
};
#endif
