#include "AlwaysInCache.hpp"
#include "FifoReadyQueue.hpp"
#include "ShortestJobFirstReadyQueue.hpp"
#include "ApproximateShortestJobFirstReadyQueue.hpp"
#include "FifoCacheAlgorithm.hpp"
#include "MostRecentlyUsedCacheAlgorithm.hpp"
#include "LeastRecentlyUsedCacheAlgorithm.hpp"
#include "SecondChanceCacheAlgorithm.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <limits>

const int Tasks = 10;
const int CSize = 5;
int Sim = 0;

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
		std::vector<float> mruLat;
		std::vector<float> mruRes;
		std::vector<float> lruLat;
		std::vector<float> lruRes;
		std::vector<float> scLat;
		std::vector<float> scRes;

		for (size_t i = 0; i < s.getJobs(); ++i)
		{
			float Lat = s.rawLatency(i);
			float Res = s.rawResponseTime(i);
			if (Sim == 0) { // fifo
				fifoLat.push_back(Lat);
				fifoRes.push_back(Res);
			}
			else if (Sim == 1) { // mru
				mruLat.push_back(Lat);
				mruRes.push_back(Res);
			}
			else if (Sim == 2) { // lru
				lruLat.push_back(Lat);
				lruRes.push_back(Res);
			}
			else if (Sim == 3) { // sc
				scLat.push_back(Lat);
				scRes.push_back(Res);
			}
		}
		if (Sim == 0) { // fifo
			std::cout << "FIFO" << std::endl;
			std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
			std::cout << "Latency : " << Average(fifoLat) << std::endl;
			std::cout << "ResponseTime : " << Average(fifoRes) << std::endl;
		}
		else if (Sim == 1) { // mru
			std::cout << "Most Recently Used" << std::endl;
			std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
			std::cout << "Latency : " << Average(mruLat) << std::endl;
			std::cout << "ResponseTime : " << Average(mruRes) << std::endl;
		}
		else if (Sim == 2) { // lru
			std::cout << "Least Recently Used" << std::endl;
			std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
			std::cout << "Latency : " << Average(lruLat) << std::endl;
			std::cout << "ResponseTime : " << Average(lruRes) << std::endl;
		}
		else if (Sim == 3) { // sc
			std::cout << "Second Chance" << std::endl;
			std::cout << "Efficiency : " << s.getEfficiency() << std::endl;
			std::cout << "Latency : " << Average(scLat) << std::endl;
			std::cout << "ResponseTime : " << Average(scRes) << std::endl;
		}
	    Sim++;
	}

  template <typename ReadyType, typename CacheType>
  void runSimulation(cs3100::SimulationParameters const& p)
  {
    cs3100::Simulation s(
      p, std::make_unique<ReadyType>(), std::make_unique<CacheType>(p.cacheSize));
    s.run();
    report(s);
  }
}

int main()
{
  /*TODO vary the simulation parameters to get richer results for your report*/

  cs3100::SimulationParameters fifo;
  fifo.cpus = 4;
  fifo.devices = 2;
  fifo.cacheSize = CSize;
  fifo.contextSwitchCost = 0.1f;
  fifo.cacheMissCost = 1.0f;
  fifo.maximumTimeSlice = std::numeric_limits<float>::max();
  fifo.jobs = Tasks;
  fifo.meanTimeBetweenJobs = 10.0f;
  fifo.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::FifoCacheAlgorithm>(fifo);

  cs3100::SimulationParameters mru;
  mru.cpus = 4;
  mru.devices = 2;
  mru.cacheSize = CSize;
  mru.contextSwitchCost = 0.1f;
  mru.cacheMissCost = 1.0f;
  mru.maximumTimeSlice = std::numeric_limits<float>::max();
  mru.jobs = Tasks;
  mru.meanTimeBetweenJobs = 10.0f;
  mru.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::MostRecentlyUsedCacheAlgorithm>(mru);

  cs3100::SimulationParameters lru;
  lru.cpus = 4;
  lru.devices = 2;
  lru.cacheSize = CSize;
  lru.contextSwitchCost = 0.1f;
  lru.cacheMissCost = 1.0f;
  lru.maximumTimeSlice = std::numeric_limits<float>::max();
  lru.jobs = Tasks;
  lru.meanTimeBetweenJobs = 10.0f;
  lru.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::LeastRecentlyUsedCacheAlgorithm>(lru);
  
  cs3100::SimulationParameters sc;
  sc.cpus = 4;
  sc.devices = 2;
  sc.cacheSize = CSize;
  sc.contextSwitchCost = 0.1f;
  sc.cacheMissCost = 1.0f;
  sc.maximumTimeSlice = std::numeric_limits<float>::max();
  sc.jobs = Tasks;
  sc.meanTimeBetweenJobs = 10.0f;
  sc.stddevTimeBetweenJobs = 2.0f;
  // create simulation with specific parameters and algorithms
  runSimulation<cs3100::FifoReadyQueue, cs3100::SecondChanceCacheAlgorithm>(sc);

  system("pause");
}
