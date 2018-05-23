#include "ReadyQueue.hpp"
#include <queue>
namespace cs3100
{
  class Simulation; // added by student
  class ApproximateShortestJobFirstReadyQueue : public ReadyQueue
  {
  public:
	ApproximateShortestJobFirstReadyQueue() :simulation(nullptr) {}
    // Approximate Shortest Job First needs to be able to look inside current
    // simulation
	void associateSimulator(Simulation* s) { simulation = s; }  // added by student
    void add(int) override;
    int next() override;

  private:
    // std::queue<int> queue;
	std::vector<int> queue;
	std::vector<float> durations; // added by student
    Simulation* simulation;
  };
}
