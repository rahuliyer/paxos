#ifndef __PAXOS_FILE_LOGGER_H__
#define __PAXOS_FILE_LOGGER_H__

#include "PaxosStateLogger.h"
#include "util/ThriftSerializer.h"
#include "util/PosixOpsIf.h"

namespace Paxos {
struct LogEntry {
  uint8_t*  buffer;
  uint8_t   committed;
  uint8_t*  checksum;
};
   
class PaxosFileLogger : public PaxosStateLogger {
public:
  PaxosFileLogger(const std::string dir, 
    const std::string prefix,
    PosixOpsIf* posix = NULL);
  void log(const PaxosTransaction&);
  bool getLatestTransaction(PaxosTransaction&);
  void commitLatestTransaction();

  void setMaxLogFileSize(uint32_t);
  ~PaxosFileLogger();

  static const uint64_t RECORD_MARKER = 0xcafebabe;

private:
  static const uint64_t MAX_FILE_SIZE = 1024 * 1024 * 1024; // 1G

  void openLatestLogFile();
  void openLogFile(unsigned long);
  void recordNextLogFile(std::string&);
  unsigned long getLatestLogFile();
  void rotateLogFileIfNeeded();

  const std::string   dir_;
  const std::string   prefix_;
  unsigned long       currentSuffix_;
  int                 currentFd_;
  std::string         currentFile_;
  uint32_t            maxSize_;
  
  // XXX: This is dirty, but will have to do for now.
  // It's the only way I can think of to mock out the posix 
  // ops for better testing
  PosixOpsIf*         posix_;
  bool                shouldDeallocatePosix_;
};
}
#endif
