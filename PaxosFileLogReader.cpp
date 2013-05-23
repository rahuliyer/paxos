#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include "util/ThriftSerializer.h"
#include "Paxos_types.h"
#include "PaxosFileLogger.h"

#include <iostream>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
using namespace Paxos;

void usage(char* name) {
  cout << name << " <log file name>" << endl;
}

void dumpLogs(void* addr, uint32_t len) {
  uint8_t* p = (uint8_t *)addr + len;

  while (p > addr) {
    uint64_t marker;
    PaxosTransaction txn;
    uint8_t committed;
    uint32_t size;
    uint32_t txn_size;

    p -= sizeof(size);
    size = *((uint32_t *)p);
    size = ntohl(size);
    p -= size;

    uint8_t* record = p;
    marker = *((uint64_t *)record);
    record += sizeof(marker);

    txn_size = size - sizeof(marker) - sizeof(committed);
    
    ThriftSerializer<PaxosTransaction> s;
    s.deserialize(txn, record, txn_size);
    record += txn_size;

    committed = *record;
    record += sizeof(committed);

    cout << "Marker: " << marker << endl;
    cout << "Proposal: " << txn.proposal << endl;
    cout << "Value: " << txn.value << endl;
    cout << "Committed: " << (bool)committed << endl;
    cout << "Txn size: " << txn_size << endl;
    cout << "Size: " << size << endl;

    cout << endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }

  int fd;
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    cerr << "Could not open file " << argv[1] << endl;
    return -1;
  }

  struct stat s;
  if (fstat(fd, &s) < 0) {
    cerr << "Could not stat file " << argv[1] << endl;
    return -1;
  }
  
  cout << (uint32_t)s.st_size << endl;

  void* addr;
  addr = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == (void *)-1) {
    perror("mmap failed");
    return -1;
  }

  dumpLogs(addr, s.st_size);

  munmap(addr, s.st_size);
  close(fd);

  return 0;
}

