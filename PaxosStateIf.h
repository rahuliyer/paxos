#ifndef __PAXOS_STATE_IF_H__
#define __PAXOS_STATE_IF_H__

#include "Paxos_types.h"

class PaxosStateIf {
public:
  virtual int64_t           getHighestProposalSeen() = 0;
  virtual void              setHighestProposalSeen(int64_t) = 0;

  virtual bool              isTransactionInProgress() = 0;

  virtual PaxosTransaction  getPendingTransaction() = 0;
  virtual void              setPendingTransaction(const PaxosTransaction&) = 0;
  virtual void              clearPendingTransaction() = 0;
};
  
#endif
