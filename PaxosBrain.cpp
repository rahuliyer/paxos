#include "PaxosBrain.h"

PaxosBrain::PaxosBrain(PaxosStateLogger& logger, 
    PaxosLearner& learner) : stateLogger_(logger), 
    learner_(learner),
    state_(stateLogger_) {
}

PaxosProposeResult PaxosBrain::recvPropose(const PaxosProposeArgs& args) {
  PaxosProposeResult res;

  if (args.proposal <= state_.getHighestProposalSeen()) {
    // We have agreed to a higher proposal
    res.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;
    res.higherProposal = state_.getHighestProposalSeen();
  } else if (state_.isTransactionInProgress()) {
    // Proposer crashed before we could respond
    res.pendingTxn = state_.getPendingTransaction();
    res.status = PaxosProposeStatus::HAS_UNFINISHED_TRANSACTION;
  } else {
    // It's all good. Let's promise not to honor any lower proposals
    state_.setHighestProposalSeen(args.proposal);
    res.status = PaxosProposeStatus::PROMISE;
  }
 
  return res;
}

PaxosAcceptResult PaxosBrain::recvAccept(const PaxosAcceptArgs& args) {
  PaxosAcceptResult res;

  if (args.txn.proposal < state_.getHighestProposalSeen()) {
    res.status = PaxosAcceptStatus::REJECTED;
  } else {
    state_.setPendingTransaction(args.txn);
   	learner_.learn(args.txn.value); 
    res.status = PaxosAcceptStatus::ACCEPTED;
  }
  return res;
}

void PaxosBrain::sentAcceptResponse() {
  // Successfully sent the response to the proposer. Clear state
  state_.clearPendingTransaction();
}

int64_t PaxosBrain::getHighestProposalSeen() {
  return state_.getHighestProposalSeen();
}

