#include "LeastRecentlyUsedCacheAlgorithm.hpp"
namespace cs3100
{
  bool LeastRecentlyUsedCacheAlgorithm::in(int x)
  {
	bool result = false;
	for (unsigned int i = 0; i < cache.size(); i++)
	{
		if (cache[i] == x)
		{
			cursor = i;
			cache.erase(cache.begin()+i);
			cache.push_back(x);
			return true;
		}
	}
	return result;
  }

  void LeastRecentlyUsedCacheAlgorithm::load(int x)
  {
	  if (cache.size() < size)
		  cache.push_back(x);
	  else
	  {
		  cache.erase(cache.begin());
		  cache.push_back(x);
	  }
  }
}
