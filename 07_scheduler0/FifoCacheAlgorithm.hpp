#include "CacheAlgorithm.hpp"
#include <queue>
namespace cs3100
{
  class FifoCacheAlgorithm : public CacheAlgorithm
  {
  public:
	FifoCacheAlgorithm(unsigned int size):CacheAlgorithm(size),cursor(0),cache(size,-1) {}
	bool in(int) override;
	void load(int) override;

  private:
	unsigned int cursor = 0;
	std::vector<int> cache;
  };
}
