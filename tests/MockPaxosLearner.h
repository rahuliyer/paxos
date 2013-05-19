#ifndef __MOCK_PAXOS_LEARNER_H__
#define __MOCK_PAXOS_LEARNER_H__

#include "gmock/gmock.h"
#include "PaxosLearner.h"

namespace Paxos {
class MockPaxosLearner : public PaxosLearner {
public:
  MOCK_METHOD1(learn, void(const std::string&));
};
}

#endif
