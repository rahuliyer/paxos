#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "PaxosBrain.h"
#include "MockPaxosLearner.h"
#include "MockPaxosState.h"

using namespace std;
using namespace testing;

TEST(SimpleProposeTest, PaxosBrainTest) {
  PaxosProposeArgs args;
  args.proposal = 1;

  MockPaxosLearner learner;
  MockPaxosState* state_ptr = new MockPaxosState();
  shared_ptr<PaxosStateIf> state(state_ptr);
  PaxosBrain brain(state, learner);

  EXPECT_CALL(*state_ptr, getHighestProposalSeen())
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_CALL(*state_ptr, setHighestProposalSeen(args.proposal))
    .Times(1);

  PaxosProposeResult res = brain.recvPropose(args);

  EXPECT_EQ(PaxosProposeStatus::PROMISE, res.status);
}

TEST(HigherProposeSeenTest, PaxosBrainTest) {
  PaxosProposeArgs args;
  args.proposal = 1;

  MockPaxosLearner learner;
  MockPaxosState* state_ptr = new MockPaxosState();
  shared_ptr<PaxosStateIf> state(state_ptr);
  PaxosBrain brain(state, learner);

  EXPECT_CALL(*state_ptr, getHighestProposalSeen())
    .Times(1)
    .WillOnce(Return(2));

  PaxosProposeResult res = brain.recvPropose(args);

  EXPECT_EQ(PaxosProposeStatus::PROMISED_HIGHER_VERSION, res.status);
  EXPECT_EQ(2, res.higherProposal);
}

TEST(UncommittedTransactionTest, PaxosBrainTest) {
  PaxosProposeArgs args;
  args.proposal = 2;

  PaxosTransaction txn;
  txn.proposal = 1;
  txn.value = "haha";

  MockPaxosLearner learner;
  MockPaxosState* state_ptr = new MockPaxosState();
  shared_ptr<PaxosStateIf> state(state_ptr);
  PaxosBrain brain(state, learner);

  EXPECT_CALL(*state_ptr, getHighestProposalSeen())
    .Times(1)
    .WillOnce(Return(1));

  EXPECT_CALL(*state_ptr, isTransactionInProgress())
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(*state_ptr, getPendingTransaction())
    .Times(1)
    .WillOnce(Return(txn));

  PaxosProposeResult res = brain.recvPropose(args);

  EXPECT_EQ(PaxosProposeStatus::HAS_UNFINISHED_TRANSACTION, res.status);
  EXPECT_EQ(1, res.pendingTxn.proposal);
  EXPECT_EQ(0, txn.value.compare(res.pendingTxn.value));
}

TEST(AcceptFailTest, PaxosBrainTest) {
  PaxosAcceptArgs args;
  args.txn.proposal = 1;
  args.txn.value = "haha";

  MockPaxosLearner learner;
  MockPaxosState* state_ptr = new MockPaxosState();
  shared_ptr<PaxosStateIf> state(state_ptr);
  PaxosBrain brain(state, learner);

  EXPECT_CALL(*state_ptr, getHighestProposalSeen())
    .Times(1)
    .WillOnce(Return(2));

  PaxosAcceptResult res = brain.recvAccept(args);

  EXPECT_EQ(PaxosAcceptStatus::REJECTED, res.status);
}

TEST(AcceptSuccessTest, PaxosBrainTest) {
  PaxosAcceptArgs args;
  args.txn.proposal = 1;
  args.txn.value = "haha";

  MockPaxosLearner learner;
  MockPaxosState* state_ptr = new MockPaxosState();
  shared_ptr<PaxosStateIf> state(state_ptr);
  PaxosBrain brain(state, learner);

  EXPECT_CALL(*state_ptr, getHighestProposalSeen())
    .Times(1)
    .WillOnce(Return(args.txn.proposal));

  EXPECT_CALL(*state_ptr, setPendingTransaction(args.txn))
    .Times(1);

  PaxosAcceptResult res = brain.recvAccept(args);

  EXPECT_EQ(PaxosAcceptStatus::ACCEPTED, res.status);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

