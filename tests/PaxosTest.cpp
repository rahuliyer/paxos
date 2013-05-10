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
    peers.push_back(&peer3);
  }

  MockPaxosPeer peer1;
  MockPaxosPeer peer2;
  MockPaxosPeer peer3;

  vector<PaxosPeer *> peers;

};

class SimpleSuccessFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    PaxosProposeResult pRes;
    PaxosAcceptResult aRes;

    pRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer3, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pRes));

    aRes.status = PaxosAcceptStatus::ACCEPTED;

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer3, sendAccept(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(aRes));
  }
};

class ProposeFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    PaxosProposeResult pRes;

    pRes.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer3, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer3, sendAccept(_, _))
      .Times(0);
  }
};

class AcceptFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    PaxosProposeResult pRes;
    PaxosAcceptResult aRes;

    pRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    EXPECT_CALL(peer3, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pRes));

    aRes.status = PaxosAcceptStatus::REJECTED;

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(aRes));

    EXPECT_CALL(peer3, sendAccept(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(aRes));
  }
};

class PartialProposeFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    PaxosProposeResult pFailRes;
    PaxosProposeResult pSuccessRes;

    pFailRes.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;
    pSuccessRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pSuccessRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pFailRes));

    EXPECT_CALL(peer3, sendPropose(_, _))
      .Times(MAX_TRIES)
      .WillRepeatedly(SetArgReferee<1>(pFailRes));

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer3, sendAccept(_, _))
      .Times(0);
  }
};

class HigherVersionFailureFixture : public BasicFixture {
  virtual void SetUpExpectations() {
    PaxosProposeResult pFailRes;
    PaxosProposeResult pSuccessRes;

    pFailRes.status = PaxosProposeStatus::PROMISED_HIGHER_VERSION;
    pSuccessRes.status = PaxosProposeStatus::PROMISE;

    EXPECT_CALL(peer1, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pSuccessRes));

    EXPECT_CALL(peer2, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pSuccessRes));

    EXPECT_CALL(peer3, sendPropose(_, _))
      .Times(1)
      .WillOnce(SetArgReferee<1>(pFailRes));

    PaxosAcceptResult aRes;
    aRes.status = PaxosAcceptStatus::REJECTED;

    EXPECT_CALL(peer1, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer2, sendAccept(_, _))
      .Times(0);

    EXPECT_CALL(peer3, sendAccept(_, _))
      .Times(0);
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

TEST_F(PartialProposeFailureFixture, BasicTest) {
 
  MockPaxosLearner learner;

  string testString = "haha";

  PaxosBrain brain(peers, learner);
  brain.setMaxRetries(MAX_TRIES - 1);
  EXPECT_FALSE(brain.submit(testString));
}

TEST_F(HigherVersionFailureFixture, BasicTest) {
 
  MockPaxosLearner learner;

  string testString = "haha";

  PaxosBrain brain(peers, learner);
  brain.setMaxRetries(0);
  EXPECT_FALSE(brain.submit(testString));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

