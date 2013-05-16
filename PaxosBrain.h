#ifndef __PAXOS_BRAIN_H__
#define __PAXOS_BRAIN_H__

#include "Paxos_types.h"
#include "PaxosThriftPeer.h"
#include "PaxosState.h"
#include "PaxosStateLogger.h"
#include "PaxosLearner.h"

class PaxosBrain {
private:
  PaxosStateLogger&             stateLogger_;
	PaxosLearner&						      learner_;
  PaxosState                    state_;

public:
  PaxosBrain(PaxosStateLogger&, PaxosLearner&);
  PaxosProposeResult  recvPropose(const PaxosProposeArgs&);
  PaxosAcceptResult   recvAccept(const PaxosAcceptArgs&);
  void                sentAcceptResponse();
  int64_t             getHighestProposalSeen();
};
#endif
