#ifndef __MOCK_POSIX_OPS_H__
#define __MOCK_POSIX_OPS_H__

#include "gmock/gmock.h"
#include "util/PosixOpsIf.h"

class MockPosixOps : public PosixOpsIf {
public:
  MOCK_METHOD2(mkdir, int(const char* path, mode_t mode));
  MOCK_METHOD3(open, int(const char *path, int flags, mode_t mode));
  MOCK_METHOD3(lseek, int64_t(int fd, off_t offset, int whence));
  MOCK_METHOD2(fstat, int(int fd, struct stat* buf));
  MOCK_METHOD3(read, int(int fd, void* buf, size_t size));
  MOCK_METHOD3(readv, int(int fd, struct iovec* iov, int count));
  MOCK_METHOD3(write, int(int fd, void* buf, size_t size));
  MOCK_METHOD3(writev, int(int fd, struct iovec* iov, int count));
  MOCK_METHOD1(close, int(int fd));
  MOCK_METHOD0(getErrno, int());
};
#endif
