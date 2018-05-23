#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <thread>

template<typename T>
class ThreadQueue
{
public:
	using func = std::function<void(void)>;
	void enqueue(T t)
	{
		std::lock_guard<std::mutex> L(M);
		Q.push(t);
	}
	bool try_dequeue(T& res)
	{
		std::lock_guard<std::mutex> L(M);
		if (Q.empty())
			return false;
		res = Q.front();
		Q.pop();
		return true;
	}
private:
	std::queue<func> Q;
	std::mutex M;
};

class ThreadPool
{
public:
	using func = std::function<void(void)>;
	ThreadPool(int n) :pool(n), queue(), itemMutex(), shouldContinue(true), taskCount(0)
	{	
	}
	~ThreadPool()
	{
	}
	void start()
	{
		for (auto&t : pool)
			t = std::thread([=] {this->run(); });
		for (unsigned int i = 0; i < pool.size(); i++)
			pool[i].join();
	}
	void stop()
	{
		shouldContinue = false;
	}
	void post(func F)
	{
		queue.enqueue(F);
		++taskCount;
	}
	void run()
	{
		while (shouldContinue)
		{
			func F;
			if (queue.try_dequeue(F))
			{
				std::unique_lock<std::mutex> L(itemMutex);
				if (!shouldContinue) return;
				{
					F();
					--taskCount;
					if (taskCount == 0)
						stop();
				}
			}
		}
	}
private:
	std::vector<std::thread> pool;
	ThreadQueue<func> queue;
	std::mutex itemMutex;
	std::atomic<bool> shouldContinue;
	std::atomic<int> taskCount;
};