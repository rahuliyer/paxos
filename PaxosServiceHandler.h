#ifndef __PAXOS_SERVICE_HANDLER_H__
#define __PAXOS_SERVICE_HANDLER_H__

#include "PaxosService.h"
#include "PaxosBrain.h"

namespace Paxos {
class PaxosServiceHandler : public PaxosServiceIf {
public:
  PaxosServiceHandler(PaxosBrain&);
  void    propose(PaxosProposeResult&, const PaxosProposeArgs&);
  void    accept(PaxosAcceptResult&, const PaxosAcceptArgs&);
  int64_t getHighestProposalSeen();

private:
  PaxosBrain& brain_;
};
}

#endif
