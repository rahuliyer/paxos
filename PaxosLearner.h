#ifndef __PAXOS_LEARNER_H__
#define __PAXOS_LEARNER_H__

namespace Paxos {
class PaxosLearner {
public:
	virtual void learn(const std::string& val) = 0;
};
}
#endif
