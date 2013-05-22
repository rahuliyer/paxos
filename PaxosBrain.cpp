#include "PaxosBrain.h"
#include "PaxosState.h"

using namespace Paxos;

PaxosBrain::PaxosBrain(PaxosStateLogger& logger, 
    PaxosLearner& learner) : learner_(learner) {
    state_.reset(new PaxosState(logger));
}

PaxosBrain::PaxosBrain(std::shared_ptr<PaxosStateIf> state, 
    PaxosLearner& learner) : learner_(learner), state_(state) {
}
  
PaxosProposeResult PaxosBrain::recvPropose(const PaxosProposeArgs& args) {
  PaxosProposeResult res;

  int64_t highestProposal = state_->getHighestProposalSeen();
  if (args.proposal <= highestProposal) {
    // We have agreed to a higher proposal
    res.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;
    res.higherProposal = highestProposal;
  } else if (state_->isTransactionInProgress()) {
    // Proposer crashed before we could respond
    res.pendingTxn = state_->getPendingTransaction();
    res.status = PaxosProposeStatus::HAS_UNFINISHED_TRANSACTION;
  } else {
    // It's all good. Let's promise not to honor any lower proposals
    state_->setHighestProposalSeen(args.proposal);
    res.status = PaxosProposeStatus::PROMISE;
  }
 
  return res;
}

PaxosAcceptResult PaxosBrain::recvAccept(const PaxosAcceptArgs& args) {
  PaxosAcceptResult res;

  if (args.txn.proposal < state_->getHighestProposalSeen()) {
    res.status = PaxosAcceptStatus::REJECTED;
  } else {
    state_->setPendingTransaction(args.txn);
    res.status = PaxosAcceptStatus::ACCEPTED;
  }
  return res;
}

void PaxosBrain::sentAcceptResponse() {
  // Successfully sent the response to the proposer. Clear state
  state_->clearPendingTransaction();
}

void PaxosBrain::learn(const std::string& value) {
  learner_.learn(value);
}

int64_t PaxosBrain::getHighestProposalSeen() {
  return state_->getHighestProposalSeen();
}

