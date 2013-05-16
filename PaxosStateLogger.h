#ifndef __PAXOS_STATE_LOGGER_H__
#define __PAXOS_STATE_LOGGER_H__

#include "Paxos_types.h"
#include "PaxosStateLoggerException.h"

class PaxosStateLogger {
public:
  virtual void log(const PaxosTransaction&) = 0;
  virtual void getLatestTransaction(PaxosTransaction&) = 0;
  virtual void commitLatestTransaction() = 0;
};

#endif
