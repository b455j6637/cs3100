#include "CacheAlgorithm.hpp"
#include <queue>
namespace cs3100
{
  class SecondChanceCacheAlgorithm : public CacheAlgorithm
  {
  public:
	  SecondChanceCacheAlgorithm(unsigned int size):CacheAlgorithm(size),cursor(0),cache() {}
	bool in(int) override;
	void load(int) override;

  private:
	unsigned int cursor = 0;
	std::vector<int> cache;
	std::vector<bool> mark;
  };
}
