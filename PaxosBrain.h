#ifndef __PAXOS_BRAIN_H__
#define __PAXOS_BRAIN_H__

#include "Paxos_types.h"
#include "PaxosThriftPeer.h"
#include "PaxosStateIf.h"
#include "PaxosStateLogger.h"
#include "PaxosLearner.h"

namespace Paxos {
class PaxosBrain {
private:
	PaxosLearner&						      learner_;
  std::shared_ptr<PaxosStateIf> state_;

public:
  PaxosBrain(PaxosStateLogger&, PaxosLearner&);
  PaxosBrain(std::shared_ptr<PaxosStateIf> state, PaxosLearner&);

  PaxosProposeResult  recvPropose(const PaxosProposeArgs&);
  PaxosAcceptResult   recvAccept(const PaxosAcceptArgs&);
  void                sentAcceptResponse();
  int64_t             getHighestProposalSeen();
};
}
#endif
