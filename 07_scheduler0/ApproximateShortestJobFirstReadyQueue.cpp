#include "ApproximateShortestJobFirstReadyQueue.hpp"
#include "Simulation.hpp"
#include <stdexcept>
#include <cfloat>
namespace cs3100
{
// average duration of each job (CPU tasks only) then choose shortest duration; i.e. past trends
  void ApproximateShortestJobFirstReadyQueue::add(int x)
  { // add to queue all possible jobs that could be scheduled to run
//    throw std::runtime_error("Shortest Job First not implemented yet");
    /*TODO Assignment 1: Correctly implement the Shortest Job First scheduling
     * algorithm*/
	queue.push_back(x);
	float sum = 0; int size = 0;
	for (unsigned int t = 0; t < simulation->jobs[x].cur; t++)
	{
		if (simulation->jobs[x].tasks[t].type == Task::Type::CPU) {
			sum += simulation->jobs[x].tasks[t].duration;
			size++;
		}
	}
	durations.push_back(sum/size);
  }

  int ApproximateShortestJobFirstReadyQueue::next()
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
