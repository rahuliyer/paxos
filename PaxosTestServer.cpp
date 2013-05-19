#include "PaxosThriftServer.h"
#include "EchoLearner.h"
#include "PaxosFileLogger.h"

#include "util/PosixOps.h"

#include <vector>
#include <thread>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
  char buf[255];
  sprintf(buf, "/tmp/paxos_log_%s", argv[1]);

  EchoLearner learner;
  PaxosFileLogger logger(buf, "log_");

  PaxosBrain brain(logger, learner);
  int port = atoi(argv[1]);
  PaxosThriftServer server(brain, port);

  cout << "Starting server on port " << port << endl;
  server.start();	

  cout << "Back in main thread!!" << endl;

  return 0;
}

