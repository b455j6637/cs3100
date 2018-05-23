#ifndef CS3100_SCHEDULER_READY_QUEUE_HPP
#define CS3100_SCHEDULER_READY_QUEUE_HPP
namespace cs3100
{
  class Simulation;  // added by student
  class ReadyQueue
  {
  public:
    ReadyQueue() {}
	virtual void associateSimulator(Simulation* s) = 0; // added by student
    virtual void add(int) = 0;
    virtual int next() = 0;
  };
}
#endif
