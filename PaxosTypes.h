#ifndef __PAXOS_TYPES_H__
#define __PAXOS_TYPES_H__
#include <sys/types.h>

#include <mutex>

struct PaxosTransaction {
  int64_t       proposal;
  std::string   value;
};

enum ProposeStatus {
  PROMISE,
  PROMISED_HIGHER_VERSION,
  HAS_UNFINISHED_TRANSACTION,
};

struct PaxosProposeArgs {
  int64_t     proposal;
};

struct PaxosProposeResult {
  ProposeStatus           status;
  PaxosTransaction        pendingTxn;
};

enum PaxosAcceptStatus {
  ACCEPTED,
  REJECTED,
};

struct PaxosAcceptArgs {
  PaxosTransaction  txn;
};

struct PaxosAcceptResult {
  PaxosAcceptStatus status;
};

#endif
