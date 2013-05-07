#ifndef __PAXOS_BRAIN_H__
#define __PAXOS_BRAIN_H__

#include "Paxos_types.h"
#include "PaxosThriftPeer.h"
#include "PaxosState.h"
#include "PaxosLearner.h"

#include <vector>

class PaxosBrain {
private:
  std::vector<PaxosPeer *>      peers_;
  PaxosState                    state_;
	PaxosLearner&						      learner_;
  int                           maxTriesPerSubmit_;

  bool sendAccept(PaxosTransaction&);

  const int MAX_RETRIES = 3;

public:
  PaxosBrain(std::vector<PaxosPeer *>&, PaxosLearner&);
	void								initializePeers();
  bool                submit(std::string& value);
  PaxosProposeResult  recvPropose(const PaxosProposeArgs&);
  PaxosAcceptResult   recvAccept(const PaxosAcceptArgs&);
  void                sentAcceptResponse();
  void                setMaxRetries(int);
};
#endif
