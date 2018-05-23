#include "AlwaysInCache.hpp"
#include "FifoReadyQueue.hpp"
#include "ShortestJobFirstReadyQueue.hpp"
#include "ApproximateShortestJobFirstReadyQueue.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <numeric>

const int Tasks = 25;
int Sim = 3;

float Average(std::vector<float> data) {
    float sum = 0;
    for (unsigned int i = 0; i < data.size(); i++)
	sum += data[i];
    float avg = sum / data.size();
    return avg;
}

namespace
{
  void report(cs3100::Simulation& s)
  {
    /*TODO create a report based on the results in s*/
    std::cout << "Tasks : " << Tasks << std::endl << std::endl;
    std::vector<float> fifoLat;
    std::vector<float> fifoRes;
    std::vector<float> roundRobinLat;
    std::vector<float> roundRobinRes;
    std::vector<float> sjfLat;
    std::vector<float> sjfRes;
    std::vector<float> asjfLat;
    std::vector<float> asjfRes;

    for (size_t i = 0; i < s.getJobs(); ++i)
    {
	float Lat = s.rawLatency(i);
	float Res = s.rawResponseTime(i);
	if (Sim == 0) { // fifo
		fifoLat.push_back(Lat);
		fifoRes.push_back(Res);
	}
	else if (Sim == 1) { // roundRobin
		roundRobinLat.push_back(Lat);
		roundRobinRes.push_back(Res);
	}
	else if (Sim == 2) { // sjf
		sjfLat.push_back(Lat);
		sjfRes.push_back(Res);
	}
	else if (Sim == 3) { // asjf
		asjfLat.push_back(Lat);
		asjfRes.push_back(Res);
	}
    }
    if (Sim == 0) { // fifo
	std::cout << "FIFO" << std::endl;
    	std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
	std::cout << "Latency : " << Average(fifoLat) << std::endl;
	std::cout << "ResponseTime : " << Average(fifoRes) << std::endl;
    }
    else if (Sim == 1) { // roundRobin
	std::cout << "Round Robin" << std::endl;
    	std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
	std::cout << "Latency : " << Average(roundRobinLat) << std::endl;
	std::cout << "ResponseTime : " << Average(roundRobinRes) << std::endl;
    }
    else if (Sim == 2) { // sjf
	std::cout << "Shortest Job First" << std::endl;
    	std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
	std::cout << "Latency : " << Average(sjfLat) << std::endl;
	std::cout << "ResponseTime : " << Average(sjfRes) << std::endl;
    }
    else if (Sim == 3) { // asjf
	std::cout << "Approximate Shortest Job First" << std::endl;
    	std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
	std::cout << "Latency : " << Average(asjfLat) << std::endl;
	std::cout << "ResponseTime : " << Average(asjfRes) << std::endl;
    }
//    Sim++;
  }

  template <typename ReadyType, typename CacheType>
  void runSimulation(cs3100::SimulationParameters const& p)
  {
    cs3100::Simulation s(
      p, std::make_unique<ReadyType>(), std::make_unique<CacheType>());
    s.run();
    report(s);
  }
}

int main()
{
  /*TODO vary the simulation parameters to get richer results for your report*/
/*  cs3100::SimulationParameters fifo;
  fifo.cpus = 4;
  fifo.devices = 2;
  fifo.cacheSize = 0;
  fifo.contextSwitchCost = 0.1f;
  fifo.cacheMissCost = 1.0f;
  fifo.maximumTimeSlice = std::numeric_limits<float>::max();
  fifo.jobs = Tasks;
  fifo.meanTimeBetweenJobs = 10.0f;
  fifo.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::AlwaysInCache>(fifo);
*/
/*  cs3100::SimulationParameters roundRobin;
  roundRobin.cpus = 4;
  roundRobin.devices = 2;
  roundRobin.cacheSize = 0;
  roundRobin.contextSwitchCost = 0.1f;
  roundRobin.cacheMissCost = 1.0f;
  roundRobin.maximumTimeSlice = 5.0f;
  roundRobin.jobs = Tasks;
  roundRobin.meanTimeBetweenJobs = 10.0f;
  roundRobin.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::AlwaysInCache>(roundRobin);
*/
/*  cs3100::SimulationParameters sjf;
  sjf.cpus = 4;
  sjf.devices = 2;
  sjf.cacheSize = 0;
  sjf.contextSwitchCost = 0.1f;
  sjf.cacheMissCost = 1.0f;
  sjf.maximumTimeSlice = 5.0f;
  sjf.jobs = Tasks;
  sjf.meanTimeBetweenJobs = 10.0f;
  sjf.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::ShortestJobFirstReadyQueue, cs3100::AlwaysInCache>(sjf);
*/
  cs3100::SimulationParameters asjf;
  asjf.cpus = 4;
  asjf.devices = 2;
  asjf.cacheSize = 0;
  asjf.contextSwitchCost = 0.1f;
  asjf.cacheMissCost = 1.0f;
  asjf.maximumTimeSlice = 5.0f;
  asjf.jobs = Tasks;
  asjf.meanTimeBetweenJobs = 10.0f;
  asjf.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::ApproximateShortestJobFirstReadyQueue, cs3100::AlwaysInCache>(asjf);

}
