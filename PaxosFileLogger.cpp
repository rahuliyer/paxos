#include "PaxosFileLogger.h"
#include "PaxosStateLoggerException.h"
#include "util/PosixOps.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include <iostream>
#include <cassert>
#include <memory>

using namespace std;
using namespace Paxos;

PaxosFileLogger::PaxosFileLogger(const string dir, 
    const string prefix,
    PosixOpsIf* posix) :
  dir_(dir), prefix_(prefix) {
  currentFile_ = "";
  currentFd_ = -1;
  currentSuffix_ = 0;
  maxSize_ = MAX_FILE_SIZE;

  if (posix) {
    posix_ = posix;
    shouldDeallocatePosix_ = false;
  } else {
    posix_ = new PosixOps();
    shouldDeallocatePosix_ = true;
  }

  openLatestLogFile();
}

void PaxosFileLogger::openLatestLogFile() {
  // Create the directory if it doesn't exist
  int ret = posix_->mkdir(dir_.c_str());
  if (ret < 0 && posix_->getErrno() != EEXIST) {
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Could not create directory " + dir_);
  }
   
  currentSuffix_ = getLatestLogFile(); 
  openLogFile(currentSuffix_);
}

unsigned long PaxosFileLogger::getLatestLogFile() {
  string curFile = dir_ + "/CurrentFile";

  // Open CurrentFile; create if it doesn't exist
  int fd = posix_->open(curFile.c_str(), O_CREAT | O_RDWR);
  if (fd < 0) {
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Could not open 'CurrentFile'");
  }

  // Read in the suffix
  unsigned long suffix = 0;
  char buf[8];
  int size;
  if ((size = posix_->read(fd, buf, sizeof(buf))) < 0) {
    posix_->close(fd);
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Error reading 'CurrentFile'");
  }

  posix_->close(fd);

  if (size != 0) {
    suffix = atoi(buf);
  }

  return suffix;
}

void PaxosFileLogger::recordNextLogFile(string& suffix) {
  string curFile = dir_ + "/CurrentFile";

  int fd = posix_->open(curFile.c_str(), O_WRONLY);
  if (fd < 0) {
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Could not open 'CurrentFile'");
  }

  // Write the name of the current log file
  if (posix_->write(fd, (void *)suffix.c_str(), suffix.size() + 1) < 0) {
    posix_->close(fd);
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Error writing 'CurrentFile'");
  }
  
  posix_->close(fd);
}

void PaxosFileLogger::openLogFile(unsigned long suffix) {
  char buf[8];

  sprintf(buf, "%04lu", suffix);
  string str_suffix(buf);

  string filename = dir_ + "/";
  filename += prefix_;
  filename += "_";
  filename += str_suffix;

  posix_->close(currentFd_);

  recordNextLogFile(str_suffix);
 
  // Open the file
  currentFd_ = posix_->open(filename.c_str(), O_CREAT | O_RDWR);
  if (currentFd_ < 0) {
    perror(__FILE__);
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::LOG_CORRUPT,
      "Could not open log file");
  }

  currentFile_ = filename;
}

void PaxosFileLogger::rotateLogFileIfNeeded() {
  struct stat s;
  if (posix_->fstat(currentFd_, &s) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not stat current log file '" + currentFile_ + "'");
  }

  if ((uint64_t)s.st_size > maxSize_) {
    currentSuffix_++;
    openLogFile(currentSuffix_++);
  }
}
 
void PaxosFileLogger::log(const PaxosTransaction& txn) {
  uint8_t committed;
  uint8_t *buf;
  uint32_t buf_size;
  unsigned long record_size;

  // Rotate the log file if it's too big
  rotateLogFileIfNeeded();
 
  ThriftSerializer<PaxosTransaction> serializer;
  serializer.serialize(txn, &buf, &buf_size);

  committed = (uint8_t)txn.committed;
  record_size = sizeof(committed) + buf_size;
  record_size = htonl(record_size);

  struct iovec iov[3];
  iov[0].iov_base = (void *)buf;
  iov[0].iov_len = buf_size;  
  
  iov[1].iov_base = &committed;
  iov[1].iov_len = sizeof(committed);

  iov[2].iov_base = &record_size;
  iov[2].iov_len = sizeof(record_size);

  if (posix_->writev(currentFd_, iov, 3) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not write to log file '" + currentFile_ + "'");
  }
}

void PaxosFileLogger::getLatestTransaction(PaxosTransaction& txn) {
  if (posix_->lseek(currentFd_, sizeof(unsigned long), SEEK_END) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not seek to latest transaction size in log file '" + 
        currentFile_ + "'");
  }

  unsigned long size = 0;
  if (posix_->read(currentFd_, &size, sizeof(unsigned long)) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not read transaction size from log file '" + currentFile_ + "'");
  }

  size = ntohl(size);
  //No logs...
  if (!size) {
    return;
  }

  if (posix_->lseek(currentFd_, size + sizeof(unsigned long), SEEK_END) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not read latest transaction in log file '" + currentFile_ + "'");
  }

  uint32_t txn_size = size - sizeof(uint8_t);
  auto deleter = [] (uint8_t* p) { free ((void *)p); };
  unique_ptr<uint8_t, decltype(deleter)> buf((uint8_t *)malloc(txn_size), 
    deleter);
  uint8_t committed;
  unsigned long read_size;

  struct iovec iov[3];
  iov[0].iov_base = buf.get();
  iov[0].iov_len = txn_size;
  iov[1].iov_base = &committed;
  iov[1].iov_len = sizeof(uint8_t);
  iov[2].iov_base = &read_size;
  iov[2].iov_len = sizeof(unsigned long);

  if (posix_->readv(currentFd_, iov, 3) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not read transaction from log file '" + currentFile_ + "'");
  }

  assert(size == htonl(read_size)); 

  ThriftSerializer<PaxosTransaction> serializer;
  serializer.deserialize(txn, buf.get(), txn_size);
  txn.committed = (bool)committed;
}

void PaxosFileLogger::commitLatestTransaction() {
  if (posix_->lseek(currentFd_, sizeof(uint8_t) + 
    sizeof(unsigned long), SEEK_END) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not seek to latest transaction size in log file '" + 
        currentFile_ + "'");
  }

  uint8_t committed = (uint8_t)true;
  if (posix_->write(currentFd_, &committed, sizeof(uint8_t)) < 0) {
    throw PaxosStateLoggerException(
      PaxosStateLoggerException::IO_ERROR,
      "Could not write commited state to latest transaction in log file '" +
        currentFile_ + "'");
  }
}

void PaxosFileLogger::setMaxLogFileSize(uint64_t size) {
  maxSize_ = size;
}

PaxosFileLogger::~PaxosFileLogger() {
  posix_->close(currentFd_);
  if (shouldDeallocatePosix_) {
    delete posix_;
  }
}

