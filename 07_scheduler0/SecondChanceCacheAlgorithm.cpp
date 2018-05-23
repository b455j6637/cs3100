#include "SecondChanceCacheAlgorithm.hpp"
namespace cs3100
{
  bool SecondChanceCacheAlgorithm::in(int x)
  {
	bool result = false;
	for (unsigned int i = 0; i < cache.size(); i++)
	{
		if (cache[i] == x)
		{
			mark[i] = true;
			return true;
		}
	}
	return result;
  }

  void SecondChanceCacheAlgorithm::load(int x)
  {
	  if (cache.size() < size)
	  {
		  cache.push_back(x);
		  mark.push_back(false);
	  }
	  else
	  {
		  while (mark[cursor]) {
			  mark[cursor] = false;
			  cursor++;
			  cursor = cursor % cache.size();
		  }
		  cache[cursor] = x;
		  cursor++;
		  cursor = cursor % cache.size();
	  }
  }
}
