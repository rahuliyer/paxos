#include "PaxosLearner.h"

#include <vector>

class EchoLearner : public PaxosLearner {
public:
	virtual void learn(const std::string& val) {
		std::cout << "Learned " << val << std::endl;
    values_.push_back(val);
	}

  void dumpVals() {
    for (auto i = values_.begin(); i != values_.end(); ++i) {
      std::cout << *i << std::endl;
    }
  }

private:
  std::vector<std::string> values_;
};

