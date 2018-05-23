// 02/11/2017 - CS 3100 - Meine, Joel
// Parallel Search and Sort

#include "threadpool.h"
#include "math.h"
#include "time.h"

#include <algorithm>	// std::sort, std::find
#include <cstring>		// std::string
#include <ctime>		// std::time
#include <fstream>		// std::ofstream
#include <iostream>		// std::cout, std::cin, std::endl
#include <mutex>		// std::mutex, std::uniqie_lock, std::lock_guard
#include <thread>		// std::thread
#include <vector>		// std::vector

const int thread_limit = 8; // maximum number of threads
int samples; // number of function execution samples
std::vector<int> log_scale = {}; // number of elements to set in a vector
const int elements_per_task = 20; // elements per thread task

std::vector<int> sort_s = {}; // result of serial sort
std::vector<int> sort_p = {}; // result of parallel sort
bool isFound_s; // is target integer found in vector using serial search?
bool isFound_p; // is target integer found in vector using parallel search?
int target; // the target integer to search for
int errorTests_sort = 0; // number of error tests run for sort
int errorTests_search = 0; // number of error tests run for sort
int errorCount_sort = 0; // number of errors detected in sort
int errorCount_search = 0; // number of errors detected in search

double time_length_s; // timer sample, serial
double time_length_p; // timer sample, parallel
std::vector<double> times_s = {}; // timer samples, serial
std::vector<double> times_p = {}; // timer samples, parallel
std::vector<double> speedups = {}; // speedup results
std::vector<double> efficiencies = {}; // efficiency results
std::vector<double> averages_ts = {}; // average times, serial
std::vector<double> stdevs_ts = {}; // standard deviations of times, serial
std::vector<double> averages_tp = {}; // average times, parallel
std::vector<double> stdevs_tp = {}; // standard deviations of times, parallel
std::vector<double> averages_s = {}; // average speedups
std::vector<double> stdevs_s = {}; // standard deviations of speedups
std::vector<double> averages_e = {}; // average efficiencies
std::vector<double> stdevs_e = {}; // standard deviations of efficiencies
unsigned cores = std::thread::hardware_concurrency(); // number of CPU cores

// quicksort, parallel
void pQuickSort(ThreadPool &t_pool, std::vector<int> &elements, int left, int right)
{
	int i = left, j = right;
	int temp;
	int pivot = elements[(left + right) / 2];
	// partition
	while (i <= j)
	{
		while (elements[i] < pivot)
			i++;
		while (elements[j] > pivot)
			j--;
		if (i <= j)
		{
			temp = elements[i];
			elements[i] = elements[j];
			elements[j] = temp;
			i++;
			j--;
		}
	}
	// recursion
	if (left < j)
		t_pool.post([&t_pool, &elements, left, j]() { pQuickSort(t_pool, elements, left, j); });
	if (i < right)
		t_pool.post([&t_pool, &elements, right, i]() {pQuickSort(t_pool, elements, i, right); });
}

// linear search, parallel
void pLinearSearch(ThreadPool &t_pool, std::vector<int> &elements, int first, int last, int integer)
{		
	for (int i = first; i <= last; i++)
	{
		if (elements[i] == integer)
		{			
			isFound_p = true;
			i = last + 1;
			t_pool.stop();
		}
	}
}

// create the log scale
void make_scale(int min_power, int max_power)
{
	for (int i = min_power; i <= max_power; i++)
		log_scale.push_back(pow(10, i));
}

// create vector of random integers
std::vector<int> rand_vector(int elements)
{
	std::vector<int> result = {}; int r;
	for (int i = 0; i < elements; i++)
	{
		r = std::rand() % elements * 2 + 1; // random number in range 1 to twice the number of elements
		result.push_back(r); // save random integer to vector
	}
	return(result);
}

// partition vector of integers for parallel search
std::vector<int> part_vector(std::vector<int> elements)
{
	std::vector<int> result = {};
	int tasks = elements.size() / elements_per_task;
	int element_id, temp;
	for (int i = 0; i < tasks; i++)
	{
		element_id = i * elements_per_task;
		result.push_back(element_id);
		temp = i + 1;
		element_id = temp * elements_per_task - 1;
		result.push_back(element_id);
	}
	return(result);
}

// run serial test
void s_run(int task, std::vector<int> elements)
{
	if (task == 0) // sort
	{
		time_length_s = timer(true); // start timer
		std::sort(elements.begin(), elements.end()); // sort the vector of integers
		time_length_s = timer(false); // end timer
		sort_s = elements; // copy result for comparison
	}
	else if (task == 1) // search
	{
		std::vector<int>::iterator result;
		target = std::rand() % elements.size() * 2 + 1; // random number in range 1 to twice the number of elements
		isFound_s = false; // initialize search result default for not found
		time_length_s = timer(true); // start timer
		result = std::find(elements.begin(), elements.end(), target); // search the vector of integers
		time_length_s = timer(false); // end timer
		if (result != elements.end())
			isFound_s = true;
	}
}

// run parallel test
void p_run(int task, std::vector<int> elements, int threads)
{
	{ // initialize threadpool
		ThreadPool t_pool(threads);
		if (task == 0) // sort
		{
			time_length_p = timer(true); // start timer
			t_pool.post([&t_pool, &elements]() {pQuickSort(t_pool, elements, 0, elements.size() - 1); });
			time_length_p = timer(false); // end timer
		}
		else if (task == 1) // search
		{
			std::vector<int> part_integers = part_vector(elements); // partition vector of random integers
			int target_p = target; // initilize target integer of search
			isFound_p = false; // initialize search result default for not found
			int first, last; // initialize search range markers
			time_length_p = timer(true); // start timer
			for (int t = 0; t < part_integers.size(); t += 2)
			{
				first = part_integers[t];
				last = part_integers[t + 1];
				t_pool.post([&t_pool, &elements, first, last, target_p]() {pLinearSearch(t_pool, elements, first, last, target_p); });
			}
			time_length_p = timer(false); // end timer
		}
	} // terminate threadpool
	if (task == 0)
		sort_p = elements; // copy result for comparison
}

void tp_header(int task, int t_number, int elements)
{
	std::cout << "task: ";
	if (task == 0)
		std::cout << "sort" << std::endl;
	else if (task == 1)
		std::cout << "search" << std::endl;
	std::cout << "thread test " << t_number + 1 << " of " << thread_limit << std::endl;
	std::cout << "elements in vector = " << elements << std::endl;
}

// run threadpool error test
void tp_errors(int task, int t_number, int elements)
{
	tp_header(task, t_number, elements);
	std::cout << "running threadpool error test... ";
	std::vector<int> integers = rand_vector(elements); // initialize vector of random integers
	s_run(task, integers);
	p_run(task, integers, t_number + 1);
	std::cout << "DONE" << std::endl;
	if (task == 0) // sort
	{
		errorTests_sort++;
		if (sort_s == sort_p)
		{
			std::cout << "PASS!" << std::endl;
			std::cout << "Serial and parallel sort results match." << std::endl;
			std::cout << std::endl;
		}
		else
		{
			errorCount_sort++;
			std::cout << "FAIL!" << std::endl;
			std::cout << "Serial and parallel sort results do not match." << std::endl;
			std::cout << "Enter key then press ENTER to continue." << std::endl;
			char fail; std::cin >> fail;
		}
	}
	else if (task == 1) // search
	{
		errorTests_search++;
		if ( isFound_s == isFound_p )
		{
			std::cout << "PASS!" << std::endl;
			std::cout << "Serial and parallel search results match." << std::endl;
			std::cout << std::endl;
		}
		else
		{
			errorCount_search++;
			std::cout << "FAIL!" << std::endl;
			std::cout << "Serial and parallel search results do not match." << std::endl;
			std::cout << "Enter key then press ENTER to continue." << std::endl;
			char fail; std::cin >> fail;
		}
	}
}

// run threadpool benchmark test
void tp_benchmark(int task, int t_number, int elements)
{
	tp_header(task, t_number, elements);
	int sample = 0;
	std::cout << "running threadpool benchmark test on " << samples << " samples... ";
	while (sample < samples)
	{
		std::vector<int> integers = rand_vector(elements); // initialize vector of random integers
		s_run(task, integers); // run the serial test
		times_s.push_back(time_length_s); // save time of serial test
		p_run(task, integers, t_number + 1); // run the parallel test
		times_p.push_back(time_length_p); // save time of parallel test
		double s = speedup(time_length_s, time_length_p); // calculate speedup
		speedups.push_back(s); // save speedup result
		double e = efficiency(s, cores); // calculate efficiency
		efficiencies.push_back(e); // save efficiency result
		sample++; // increment to next sample
	}
	std::cout << "DONE" << std::endl;

	std::cout << "average, time (serial _ parallel) = (" << average(times_s) << " _ " << average(times_p) << ") milliseconds" << std::endl;
	averages_ts.push_back(average(times_s)); averages_tp.push_back(average(times_p)); // save average times
	std::cout << "standard deviation, time (serial _ parallel) = (" << stdev(times_s) << " _ " << stdev(times_p) << ") milliseconds" << std::endl;
	stdevs_ts.push_back(stdev(times_s)); stdevs_tp.push_back(stdev(times_p)); // save standard deviation of times

	std::cout << "average, (speedup _ efficiency) = (" << average(speedups) << " _ " << average(efficiencies) << ")" << std::endl;
	averages_s.push_back(average(speedups)); averages_e.push_back(average(efficiencies)); // save average speedups and efficiencies
	std::cout << "standard deviation, (speedup _ efficiency) = (" << stdev(speedups) << " _ " << stdev(efficiencies) << ")" << std::endl;
	stdevs_s.push_back(stdev(speedups)); stdevs_e.push_back(stdev(efficiencies)); // save standard deviation of speedups and efficiencies

	// clear data for next test
	times_s = {}; times_p = {};
	speedups = {}; efficiencies = {};
	std::cout << std::endl;
}

// clear data for next task test
void clear_data()
{
	averages_ts = {}; // average times, serial
	stdevs_ts = {}; // standard deviations of times, serial
	averages_tp = {}; // average times, parallel
	stdevs_tp = {}; // standard deviations of times, parallel
	averages_s = {}; // average speedups
	stdevs_s = {}; // standard deviations of speedups
	averages_e = {}; // average efficiencies
	stdevs_e = {}; // standard deviations of efficiencies
}

// write performance report
void w_report(std::string filename)
{
	std::cout << "writing performance report... ";
	std::ofstream report(std::string(filename + ".txt").c_str());
	if (report.is_open())
	{
		report << std::string("task: " + filename).c_str() << std::endl;
		report << "CPU cores = " << cores << std::endl;
		report << "process threads, max number of = " << thread_limit << std::endl;
		report << "elements per task = " << elements_per_task << std::endl;
		report << "samples, number of per test = " << samples << "\n" << std::endl;

		report << "t = time, execution (ms)" << std::endl;
		report << "sl = serial" << std::endl;
		report << "pl = parallel" << std::endl;
		report << "s = speedup" << std::endl;
		report << "e = efficiency" << "\n" << std::endl;

		report << "The data is formatted to CSV standard." << "\n" << std::endl;

		report << "threads,elements,average_t.sl,average_t.pl,stdev_t.sl,stdev_t.pl,average_s,stdev_s,average_e,stdev_e" << "\n" << std::endl;
		for (int t = 1; t < thread_limit; t++) // for every thread
		{
			for (int l = 0; l < log_scale.size(); l++) // for every value of log scale
				report << t + 1 << "," << log_scale[l]
				<< "," << averages_ts[l] << "," << averages_tp[l]
				<< "," << stdevs_ts[l] << "," << stdevs_tp[l]
				<< "," << averages_s[l] << "," << stdevs_s[l]
				<< "," << averages_e[l] << "," << stdevs_e[l] << std::endl;
		}
		report.close();
	}
	std::cout << "DONE" << "\n" << std::endl;
}

int main()
{
	std::srand(time(0)); // initiate random seed
	make_scale(2, 6); // initialize the log scale
	// run threadpool error test
	for (int T = 0; T < 2; T++) // for sort and search tasks; T=0 is sort, T=1 is search
	{
		for (int t = 1; t < thread_limit; t++) // for every thread
		{
			for (int l = 0; l < log_scale.size(); l++) // for every value of log scale
				tp_errors(T, t, log_scale[l]); // run error test
		}
	}

	std::cout << std::endl;
	std::cout << "ERROR TEST COMPLETE!" << std::endl;
	std::cout << std::endl;
	std::cout << "Errors, number of in sort = " << errorCount_sort << " out of " << errorTests_sort << " tests run." << std::endl;
	std::cout << "Errors, number of in search = " << errorCount_search << " out of " << errorTests_search << " tests run." << std::endl;
	std::cout << std::endl;
	std::cout << "Enter key then press ENTER to run benchmark tests." << std::endl;
	char stop_error; std::cin >> stop_error;

	// ask user for number of samples
	std::cout << std::endl;
	std::cout << "samples, number of per thread per elements (min = 4): ";
	std::cin >> samples; std::cout << std::endl;
	if (samples < 4) // if not enough samples
		samples = 4; // then set to minimum

	std::string task_name;
	// run threadpool benchmark test: task=0 for sort
	for (int t = 1; t < thread_limit; t++) // for every thread
	{
		for (int l = 0; l < log_scale.size(); l++) // for every value of log scale
			tp_benchmark(0, t, log_scale[l]); // run benchmark test
	}
	task_name = "sort";
	w_report(task_name); // write performance report
	clear_data();

	// run threadpool benchmark test: task=1 for search
	for (int t = 1; t < thread_limit; t++) // for every thread
	{
		for (int l = 0; l < log_scale.size(); l++) // for every value of log scale
			tp_benchmark(1, t, log_scale[l]); // run benchmark test
	}
	task_name = "search";
	w_report(task_name); // write performance report
	clear_data();

	std::cout << std::endl;
	std::cout << "BENCHMARK TEST COMPLETE!" << std::endl;
	std::cout << std::endl;
	std::cout << "Results have been written to files." << std::endl;
	std::cout << std::endl;
	std::cout << "Enter key then press ENTER to end program." << std::endl;
	char stop_benchmark; std::cin >> stop_benchmark;

	return 0;
}