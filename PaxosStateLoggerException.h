#ifndef __PAXOS_STATE_LOGGER_EXCEPTION_H__
#define __PAXOS_STATE_LOGGER_EXCEPTION_H__

namespace Paxos {
class PaxosStateLoggerException : public std::exception {
public:
  static const int LOG_NOT_FOUND = 0;
  static const int LOG_CORRUPT = 1;
  static const int IO_ERROR = 3;

  PaxosStateLoggerException(int errorcode, const std::string what) : 
    errcode_(errorcode), what_(what) {
  }

  int errcode() { return errcode_; }
  const char * what() { return what_.c_str(); }

private:
  const int         errcode_;
  const std::string what_;
};
}
#endif
