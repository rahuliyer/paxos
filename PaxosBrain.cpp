#include "PaxosBrain.h"

PaxosBrain::PaxosBrain(std::vector<PaxosPeer>& peers) {
  peers_ = peers;
}

void PaxosBrain::initializePeers() {
	for (auto i = peers_.begin(); i != peers_.end(); ++i) {
		i->initialize();
	}
}

PaxosProposeResult PaxosBrain::recvPropose(const PaxosProposeArgs& args) {
  PaxosProposeResult res;

  if (args.proposal <= state_.getHighestProposalSeen()) {
    // We have agreed to a higher proposal
    res.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;
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
    // learn
    res.status = PaxosAcceptStatus::ACCEPTED;
  }
  return res;
}

void PaxosBrain::sentAcceptResponse() {
  // Successfully sent the response to the proposer. Clear state
  state_.clearPendingTransaction();
}

bool PaxosBrain::sendAccept(PaxosTransaction& p) {
  PaxosAcceptArgs args;
  args.txn = p;
  std::vector<PaxosAcceptResult> a_res;

  for (auto i = peers_.begin(); i < peers_.end(); ++i) {
    try {
      PaxosAcceptResult par;
      i->sendAccept(args, par);
      a_res.push_back(par);
    } catch (...) {
      continue;
    }
  }

  if (a_res.size() <= peers_.size() / 2) {
    return false;
  }

  for (auto i = a_res.begin(); i < a_res.end(); ++i) {
    if (i->status == PaxosAcceptStatus::REJECTED) {
      return false;
    }
  }

  return true;
}

void PaxosBrain::submit(std::string& val) {
  bool success = false;

  while (!success) {
    PaxosProposeArgs p_args;

    p_args.proposal = state_.getHighestProposalSeen() + 1;

    std::vector<PaxosProposeResult> p_res;
    for (auto i = peers_.begin(); i != peers_.end(); ++i) {
      try {
        PaxosProposeResult ppr;
        i->sendPropose(p_args, ppr);
        p_res.push_back(ppr);
      } catch (...) {
        continue;
      }
    }

    // Did enough peers respond?
    if (p_res.size() <= peers_.size() / 2) {
      // Nope... do over!
      continue;
    }

    unsigned int numPromises = 0;
    bool shouldSendAccept = true;
    PaxosTransaction highestPendingTxn;
    highestPendingTxn.proposal = -1;
    for (auto i = p_res.begin(); i != p_res.end(); ++i) {
      if (i->status == PaxosProposeStatus::PROMISE) {
        numPromises++;
        continue;
      } else if (i->status == PaxosProposeStatus::PROMISED_HIGHER_VERSION) {
        // Someone had a higher proposal. Bail...
        shouldSendAccept = false;
        break;
      } else {
        // Some peer had an uncommitted transaction
        if (i->pendingTxn.proposal > highestPendingTxn.proposal) {
          highestPendingTxn = i->pendingTxn;
        }
      }
    }

    if (!shouldSendAccept) {
      // There is a higher proposal than ours. Do over
      continue;
    }

    if (highestPendingTxn.proposal == -1) {
      // No pending proposals!
      highestPendingTxn.proposal = p_args.proposal;
      highestPendingTxn.value = val;
    }
      
    bool status = sendAccept(highestPendingTxn);

    if (status && highestPendingTxn.proposal == p_args.proposal) {
      // We successfully agreed upon our value
      success = true;
    }

    // We recovered a previous transaction. Do over
  }
}

