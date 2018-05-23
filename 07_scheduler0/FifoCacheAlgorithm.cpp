#include "FifoCacheAlgorithm.hpp"
namespace cs3100
{
  bool FifoCacheAlgorithm::in(int x)
  {
	bool result = false;
	for (unsigned int i = 0; i < cache.size(); i++)
	{
		if (cache[i] == x)
			result = true;
	}
	return result;
  }

  void FifoCacheAlgorithm::load(int x)
  {
	  cache[cursor] = x;
	  cursor++;
	  if (cursor == cache.size() - 1)
		  cursor = 0;
  }
}
