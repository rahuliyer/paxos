#include "MockPosixOps.h"
#include "PaxosFileLogger.h"

using namespace testing;
using namespace Paxos;

typedef int CopyBufferMethod(int, void*, unsigned long);

class CopyBufferAction : public ActionInterface<CopyBufferMethod> {
public:
  CopyBufferAction(void* buf) {
    buf_ = buf;
  }

  virtual int Perform(const std::tr1::tuple<int, void*, unsigned long>& args) {
    void* dest = std::tr1::get<1>(args);
    int len = std::tr1::get<2>(args);

    memcpy(dest, buf_, len);
    return len;
  }

private:
  void* buf_;
};

Action<CopyBufferMethod> CopyBuffer(void* buf) {
  return MakeAction(new CopyBufferAction(buf));
}

TEST(ConstructorMkdirFailTest, PaxosFileLoggerTest) {
  MockPosixOps posix;

  EXPECT_CALL(posix, mkdir(StrEq("/tmp/paxos"), _))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(PaxosFileLogger logger("/tmp/paxos", "log_", &posix), 
    PaxosStateLoggerException);
}

TEST(ConstructorMkdirExistsTest, PaxosFileLoggerTest) {
  MockPosixOps posix;

  EXPECT_CALL(posix, mkdir(StrEq("/tmp/paxos"), _))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_CALL(posix, getErrno())
    .Times(1)
    .WillOnce(Return(EEXIST));

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/CurrentFile"), O_RDWR | O_CREAT, _))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(PaxosFileLogger logger("/tmp/paxos", "log_", &posix), 
    PaxosStateLoggerException);
}

TEST(ConstructorOpenFailTest, PaxosFileLoggerTest) {
  MockPosixOps posix;

  EXPECT_CALL(posix, mkdir(StrEq("/tmp/paxos"), _))
    .Times(1)
    .WillOnce(Return(3));

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/CurrentFile"), O_RDWR | O_CREAT, _))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(PaxosFileLogger logger("/tmp/paxos", "log_", &posix), 
    PaxosStateLoggerException);
}


TEST(ConstructorReadFailTest, PaxosFileLoggerTest) {
  MockPosixOps posix;

  EXPECT_CALL(posix, mkdir(StrEq("/tmp/paxos"), _))
    .Times(1)
    .WillOnce(Return(3));

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/CurrentFile"), O_RDWR | O_CREAT, _))
    .Times(1)
    .WillOnce(Return(4));

  EXPECT_CALL(posix, read(4, _, _))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(PaxosFileLogger logger("/tmp/paxos", "log_", &posix), 
    PaxosStateLoggerException);
}


int writePtrMatcher(void* buf) {
  return !memcmp(buf, (void*)"0000", 5);
}

TEST(ConstructorSuccessTest, PaxosFileLoggerTest) {
  MockPosixOps posix;

  EXPECT_CALL(posix, mkdir(StrEq("/tmp/paxos"), _))
    .Times(1)
    .WillOnce(Return(3));

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/CurrentFile"), O_RDWR | O_CREAT, _))
    .Times(1)
    .WillOnce(Return(4));

  char buf[] = "0000";
  EXPECT_CALL(posix, read(4, _, _))
    .Times(1)
    .WillOnce(CopyBuffer(buf));

  EXPECT_CALL(posix, close(4))
    .Times(2);

  EXPECT_CALL(posix, close(-1))
    .Times(1);

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/CurrentFile"), O_WRONLY, _))
    .Times(1)
    .WillOnce(Return(4));

  EXPECT_CALL(posix, write(4, Truly(writePtrMatcher), 5))
    .Times(1)
    .WillOnce(Return(5));

  EXPECT_CALL(posix, open(StrEq("/tmp/paxos/log__0000"), O_CREAT | O_RDWR, _))
    .Times(1)
    .WillOnce(Return(5));

  // Destructor will close it
  EXPECT_CALL(posix, close(5))
    .Times(1);

  PaxosFileLogger logger("/tmp/paxos", "log_", &posix);
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

