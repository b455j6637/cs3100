#ifndef CS3100_SCHEDULER_FIFO_READY_QUEUE_HPP
#define CS3100_SCHEDULER_FIFO_READY_QUEUE_HPP

#include "ReadyQueue.hpp"
#include <queue>

namespace cs3100
{
  class Simulation; // added by student
  class FifoReadyQueue : public ReadyQueue
  {
  public:
	void associateSimulator(Simulation* s) { simulation = s; } // added by student
    void add(int) override;
    int next() override;

  private:
    std::queue<int> queue;
	Simulation* simulation;
  };
}

#endif
