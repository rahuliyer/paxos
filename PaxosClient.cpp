#include "PaxosClient.h"
using namespace std;

PaxosClient::PaxosClient(vector<PaxosPeer *>& peers) {
  peers_ = peers;
  highestProposalSeen_ = 0;
  maxTriesPerSubmit_ = MAX_RETRIES;
}

void PaxosClient::initialize() {
  std::lock_guard<std::mutex> g(clientLock_);
  for (auto i = peers_.begin(); i != peers_.end(); ++i) {
    (*i)->initialize();
    
    int highestProposalSeen = (*i)->getHighestProposalSeen();
    if (highestProposalSeen > highestProposalSeen_) {
      highestProposalSeen_ = highestProposalSeen;
    }
  }
}

void PaxosClient::setMaxRetries(int max_retries) {
  maxTriesPerSubmit_ = max_retries + 1;
}

bool PaxosClient::sendAccept(PaxosTransaction& p) {
  PaxosAcceptArgs args;
  args.txn = p;
  std::vector<PaxosAcceptResult> a_res;

  for (auto i = peers_.begin(); i < peers_.end(); ++i) {
    try {
      PaxosAcceptResult par;
      (*i)->sendAccept(args, par);
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

bool PaxosClient::submit(std::string& val) {
  bool success = false;
  int tries = 0;

  while (!success && tries < maxTriesPerSubmit_) {
    ++tries;
    PaxosProposeArgs p_args;

    p_args.proposal = ++highestProposalSeen_;

    std::vector<PaxosProposeResult> p_res;
    for (auto i = peers_.begin(); i != peers_.end(); ++i) {
      try {
        PaxosProposeResult ppr;
        (*i)->sendPropose(p_args, ppr);
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
    bool isReplaying = false;
    PaxosTransaction highestPendingTxn;
    highestPendingTxn.proposal = -1;
    for (auto i = p_res.begin(); i != p_res.end(); ++i) {
      if (i->status == PaxosProposeStatus::PROMISE) {
        numPromises++;
        continue;
      } else if (i->status == PaxosProposeStatus::PROMISED_HIGHER_VERSION) {
        // Someone had a higher proposal. Bail...
        shouldSendAccept = false;
        if (highestProposalSeen_ < i->higherProposal) {
          // This can happen if multiple client respond with 
          // PROMISED_HIGHER_VERSION
          highestProposalSeen_ = i->higherProposal;
        }
        break;
      } else {
        // Some peer had an uncommitted transaction
        if (i->pendingTxn.proposal > highestPendingTxn.proposal) {
          highestPendingTxn = i->pendingTxn;
        }

        isReplaying = true;
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
    
    if (isReplaying) {
      isReplaying = false;
      // We recovered a previous transaction, so success stays false; Do over
    } else {
      if (status && highestPendingTxn.proposal == p_args.proposal) {
        // We successfully agreed upon our value
        success = true;
      }
    }
  }

  return success;
}

