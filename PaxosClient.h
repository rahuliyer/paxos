#ifndef __PAXOS_CLIENT_H__
#define __PAXOS_CLIENT_H__

#include "PaxosPeer.h"
#include "thrift/gen-cpp/Paxos_types.h"

#include <vector>
#include <mutex>

class PaxosClient {
public:
  PaxosClient(std::vector<PaxosPeer *>& peers);
  void initialize();
  void setMaxRetries(int);
  bool submit(std::string& val);

private:
  const int   MAX_RETRIES = 3;

  bool sendAccept(PaxosTransaction&);

  int                      maxTriesPerSubmit_;
  std::vector<PaxosPeer *> peers_;
  int64_t                  highestProposalSeen_;
  std::mutex               clientLock_;
};

#endif
