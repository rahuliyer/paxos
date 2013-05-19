#ifndef __MOCK_PAXOS_STATE_H__
#define __MOCK_PAXOS_STATE_H__

#include "PaxosStateIf.h"

class MockPaxosState : public PaxosStateIf {
public:
  MOCK_METHOD0(getHighestProposalSeen, int64_t());
  MOCK_METHOD1(setHighestProposalSeen, void(int64_t));
  MOCK_METHOD0(isTransactionInProgress, bool());
  MOCK_METHOD0(getPendingTransaction, PaxosTransaction());
  MOCK_METHOD1(setPendingTransaction, void(const PaxosTransaction&));
  MOCK_METHOD0(clearPendingTransaction, void());
};

#endif
