#include "ShortestJobFirstReadyQueue.hpp"
#include "Simulation.hpp"
#include <stdexcept>
#include <cfloat>
namespace cs3100
{
  void ShortestJobFirstReadyQueue::associateSimulator(Simulation* s)
  {
	  simulation = s;
  }
// total durations of each job then choose shortest duration; i.e. future projection
  void ShortestJobFirstReadyQueue::add(int x)
  { // add to queue all possible jobs that could be scheduled to run
//    throw std::runtime_error("Shortest Job First not implemented yet");
    /*TODO Assignment 1: Correctly implement the Shortest Job First scheduling
     * algorithm*/
	queue.push_back(x);
	float sum = 0;
	for (unsigned int t = simulation->jobs[x].cur; t < simulation->jobs[x].tasks.size(); t++)
		sum += simulation->jobs[x].tasks[t].duration;
	durations.push_back(sum);
  }

  int ShortestJobFirstReadyQueue::next()
  { // select job to run that contains lowest sum of task durations
//    throw std::runtime_error("Shortest Job First not implemented yet");
    /*TODO Assignment 1: Correctly implement the Shortest Job First scheduling
     * algorithm*/
	if (queue.empty()) return -1;
	float shortest = FLT_MAX; int x = 0;
	for (unsigned int d = 0; d < durations.size(); d++)
	{
		if (durations[d] < shortest)
		{
			shortest = durations[d];
			x = d;
		}
	}
	int result = queue[x];
	queue.erase(queue.begin() + x);
	durations.erase(durations.begin() + x);
	return result;
  }
}
