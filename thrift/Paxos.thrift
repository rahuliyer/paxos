struct PaxosTransaction {
  1: i64      proposal,
  2: binary   value,
  3: bool     committed,
}

enum PaxosProposeStatus {
  PROMISE,
  PROMISED_HIGHER_VERSION,
  HAS_UNFINISHED_TRANSACTION,
}

struct PaxosProposeArgs {
  1: i64 proposal,
}

struct PaxosProposeResult {
  1: PaxosProposeStatus             status,
  2: optional i64                   higherProposal,                        
  3: optional PaxosTransaction      pendingTxn,
}

enum PaxosAcceptStatus {
  ACCEPTED,
  REJECTED,
}

struct PaxosAcceptArgs {
  1: PaxosTransaction    txn,
}

struct PaxosAcceptResult {
  1: PaxosAcceptStatus   status,
}

service PaxosService {
  PaxosProposeResult  propose(1: PaxosProposeArgs pArgs),
  PaxosAcceptResult   accept(1: PaxosAcceptArgs aArgs),
  i64                 getHighestProposalSeen(),
}
 

