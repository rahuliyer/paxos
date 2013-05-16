#ifndef __POSIX_OPS_IF_H__
#define __POSIX_OPS_IF_H__

// large file support
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>

class PosixOpsIf {
public:
  static const mode_t defaultFileMode_ = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  static const mode_t defaultDirMode_ = 
    S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; 
  virtual int mkdir(const char* path, 
    mode_t mode = PosixOpsIf::defaultDirMode_) = 0;
  virtual int open(const char *path, int flags, 
    mode_t mode = PosixOpsIf::defaultFileMode_) = 0;
  virtual int64_t lseek(int fd, off_t offset, int whence) = 0;
  virtual int fstat(int fd, struct stat* buf) = 0;
  virtual int read(int fd, void* buf, size_t size) = 0;
  virtual int readv(int fd, struct iovec* iov, int count) = 0;
  virtual int write(int fd, void* buf, size_t size) = 0;
  virtual int writev(int fd, struct iovec* iov, int count) = 0;
  virtual int close(int fd) = 0;
  virtual int getErrno() = 0;

  virtual ~PosixOpsIf() {}
};
#endif
