#ifndef __PAXOS_BRAIN_H__
#define __PAXOS_BRAIN_H__

#include "Paxos_types.h"
#include "PaxosPeer.h"
#include "PaxosState.h"

#include <vector>

class PaxosBrain {
private:
  std::vector<PaxosPeer>  peers_;
  PaxosState              state_;

  bool sendAccept(PaxosTransaction&);

public:
  PaxosBrain(std::vector<PaxosPeer>&);
  void                submit(std::string& value);
//  void                registerLearner(void (*)(std::string&);
  PaxosProposeResult  recvPropose(const PaxosProposeArgs&);
  PaxosAcceptResult   recvAccept(const PaxosAcceptArgs&);
  void                sentAcceptResponse();
};
#endif
