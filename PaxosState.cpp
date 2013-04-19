#include "PaxosState.h"

PaxosState::PaxosState() {
  highestProposalSeen_ = -1;
  txnInProgress_ = false;
}

int64_t PaxosState::getHighestProposalSeen() {
  std::lock_guard<std::mutex> g(stateLock_);
  return highestProposalSeen_;
}

void PaxosState::setHighestProposalSeen(int64_t proposal) {
  std::lock_guard<std::mutex> g(stateLock_);
  highestProposalSeen_ = proposal;
}

bool PaxosState::isTransactionInProgress() {
  std::lock_guard<std::mutex> g(stateLock_);
  return txnInProgress_;
}

PaxosTransaction PaxosState::getPendingTransaction() {
  std::lock_guard<std::mutex> g(stateLock_);
  return pendingTxn_;
}

void PaxosState::setPendingTransaction(const PaxosTransaction& txn) {
  std::lock_guard<std::mutex> g(stateLock_);
  pendingTxn_ = txn;
  txnInProgress_ = true;
}


void PaxosState::clearPendingTransaction() {
  std::lock_guard<std::mutex> g(stateLock_);
  txnInProgress_ = false;
}
