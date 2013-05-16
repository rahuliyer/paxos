#include "PaxosThriftPeer.h"
#include "PaxosThriftServer.h"
#include "EchoLearner.h"
#include "PaxosClient.h"
#include "PaxosFileLogger.h"

#include "util/PosixOps.h"

#include <vector>
#include <thread>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
  std::vector<PaxosPeer *> peers;
  for (int port = 9090; port < 9093; ++port) {
    PaxosThriftPeer *p = new PaxosThriftPeer("localhost", port);
    peers.push_back(p);
  }

  char buf[255];
  sprintf(buf, "/tmp/paxos_log_%s", argv[1]);

  EchoLearner learner;
  PaxosFileLogger logger(buf, "log_");
  logger.setMaxLogFileSize(600);

  PaxosBrain brain(logger, learner);
  int port = atoi(argv[1]);
  PaxosThriftServer server(brain, port);

  cout << "Starting server on port " << port << endl;
  server.start();	

  cout << "Back in main thread!!" << endl;

  sleep(5);

  if (argc == 3) {
    PaxosClient client(peers);
    client.initialize();

    for (int i = 0; i < 10; ++i) {
      stringstream ss;
      string s;
      ss << i;
      s = ss.str();
      cout << "Submitting " << s << endl;
      client.submit(s);
      sleep(2);
    }

    learner.dumpVals();
  }

  return 0;
}

