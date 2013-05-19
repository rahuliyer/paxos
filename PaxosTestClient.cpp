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

  PaxosClient client(peers);
  client.initialize();

  for (int i = 0; i < 100000; ++i) {
    stringstream ss;
    string s;
    ss << i;
    s = ss.str();
    cout << "Submitting " << s << endl;
    client.submit(s);
//    sleep(2);
  }

  return 0;
}

