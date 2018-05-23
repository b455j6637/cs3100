#include "MostRecentlyUsedCacheAlgorithm.hpp"
namespace cs3100
{
  bool MostRecentlyUsedCacheAlgorithm::in(int x)
  {
	bool result = false;
	for (unsigned int i = 0; i < cache.size(); i++)
	{
		if (cache[i] == x)
		{
			cursor = i;
			return true;
		}
	}
	return result;
  }

  void MostRecentlyUsedCacheAlgorithm::load(int x)
  {
	  if (cache.size() < size)
		  cache.push_back(x);
	  else
		  cache[cursor] = x;
  }
}
