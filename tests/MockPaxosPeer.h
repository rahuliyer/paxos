#ifndef __MOCK_PAXOS_PEER_H__
#define __MOCK_PAXOS_PEER_H__

#include "gmock/gmock.h"
#include "PaxosPeer.h"

namespace Paxos {
class MockPaxosPeer : public PaxosPeer {
public:
  MOCK_METHOD2(sendPropose, void(const PaxosProposeArgs&, PaxosProposeResult&));
  MOCK_METHOD2(sendAccept, void(const PaxosAcceptArgs&, PaxosAcceptResult&));
  MOCK_METHOD0(getHighestProposalSeen, int64_t());
};
}

#endif
