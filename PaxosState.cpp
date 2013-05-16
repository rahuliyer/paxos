#include "PaxosState.h"

using namespace std;

PaxosState::PaxosState(PaxosStateLogger& logger) : logger_(logger) {
  highestProposalSeen_ = -1;
  txnInProgress_ = false;

  PaxosTransaction txn;
  
  try {
    logger_.getLatestTransaction(txn);
  
    highestProposalSeen_ = txn.proposal;
    if (!txn.committed) {
      pendingTxn_ = txn;
      txnInProgress_ = true;
    }
  } catch (PaxosStateLoggerException& e) {
    if (e.errcode() != PaxosStateLoggerException::LOG_NOT_FOUND) {
      cerr << e.what() << endl;
      throw;
    }
  }
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
  logger_.log(txn);
  txnInProgress_ = true;
}


void PaxosState::clearPendingTransaction() {
  std::lock_guard<std::mutex> g(stateLock_);
  logger_.commitLatestTransaction();
  txnInProgress_ = false;
}
