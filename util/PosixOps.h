#ifndef __POSIX_OPS_H__
#define __POSIX_OPS_H__

// large file support
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>

#include "PosixOpsIf.h"

class PosixOps : public PosixOpsIf {
public:
  virtual inline int mkdir(const char* path, 
      mode_t mode = PosixOpsIf::defaultDirMode_) {
    return ::mkdir(path, mode);
  }

  inline int open(const char *path, 
      int flags, 
      mode_t mode = PosixOpsIf::defaultFileMode_) {
    return ::open(path, flags, mode);
  }

  inline int64_t lseek(int fd, off_t offset, int whence) {
    return (int64_t)::lseek(fd, offset, whence);
  }

  inline int fstat(int fd, struct stat* buf) {
    return ::fstat(fd, buf);
  }
  
  inline int read(int fd, void* buf, size_t size) {
    return ::read(fd, buf, size);
  }

  inline int readv(int fd, struct iovec* iov, int count) {
    return ::readv(fd, iov, count);
  }

  inline int write(int fd, void* buf, size_t size) {
    return ::write(fd, buf, size);
  }

  inline int writev(int fd, struct iovec* iov, int count) {
    return ::writev(fd, iov, count);
  }
 
  inline int close(int fd) {
    return ::close(fd);
  }
  
  inline int getErrno() {
    return errno;
  }
};
#endif
