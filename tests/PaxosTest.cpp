#include "gtest/gtest.h"

#include "MockPaxosPeer.h"
#include "MockPaxosLearner.h"
#include "PaxosBrain.h"

#include <vector>
#include <string>

#define MAX_TRIES 3

using namespace std;
using namespace testing;

class BasicFixture : public Test {
protected:
  virtual void SetUpExpectations() = 0;
  virtual void SetUp() {
    SetUpExpectations();

    peers.push_back(&peer1);
    peers.push_back(&peer2);
  }

  MockPaxosPeer peer1;
  MockPaxosPeer peer2;

  vector<PaxosPeer *> peers;

  PaxosProposeResult pRes;
  PaxosAcceptResult aRes;
};

class SimpleSuccessFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    pRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pRes));

    aRes.status = PaxosAcceptStatus::ACCEPTED;

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(aRes));
  }
};

class ProposeFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    pRes.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(0);
  }
};

class AcceptFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    pRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    aRes.status = PaxosAcceptStatus::REJECTED;

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(aRes));
  }
};

TEST_F(SimpleSuccessFixture, BasicTest) {
 
  MockPaxosLearner learner;

  string testString = "haha";

  PaxosBrain brain(peers, learner);
  EXPECT_TRUE(brain.submit(testString));
}

TEST_F(ProposeFailureFixture, BasicTest) {
 
  MockPaxosLearner learner;

  string testString = "haha";

  PaxosBrain brain(peers, learner);
  brain.setMaxRetries(MAX_TRIES - 1);
  EXPECT_FALSE(brain.submit(testString));
}

TEST_F(AcceptFailureFixture, BasicTest) {
 
  MockPaxosLearner learner;

  string testString = "haha";

  PaxosBrain brain(peers, learner);
  brain.setMaxRetries(MAX_TRIES - 1);
  EXPECT_FALSE(brain.submit(testString));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

