#ifndef __PAXOS_STATE_H__
#define __PAXOS_STATE_H__

#include "Paxos_types.h"
#include "PaxosStateIf.h"
#include "PaxosStateLogger.h"

#include <mutex>

namespace Paxos {
class PaxosState : public PaxosStateIf {
private:
  std::mutex        stateLock_;
  int64_t           highestProposalSeen_;
  bool              txnInProgress_;
  PaxosTransaction  pendingTxn_;

  PaxosStateLogger& logger_;

public:
  PaxosState(PaxosStateLogger&);
  int64_t             getHighestProposalSeen();
  void                setHighestProposalSeen(int64_t);

  bool                isTransactionInProgress();

  PaxosTransaction    getPendingTransaction();
  void                setPendingTransaction(const PaxosTransaction&);
  void                clearPendingTransaction();
};
}  
#endif
