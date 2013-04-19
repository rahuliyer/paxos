#ifndef __PAXOS_SERVICE_HANDLER_H__
#define __PAXOS_SERVICE_HANDLER_H__

#include "PaxosService.h"
#include "PaxosBrain.h"

class PaxosServiceHandler : public PaxosServiceIf {
public:
  PaxosServiceHandler(PaxosBrain&);
  void propose(PaxosProposeResult&, const PaxosProposeArgs&);
  void accept(PaxosAcceptResult&, const PaxosAcceptArgs&);

private:
  PaxosBrain& brain_;
};

#endif
