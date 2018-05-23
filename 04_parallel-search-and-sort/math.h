#include <cmath>		// std::pow, std::sqrt
#include <cstdlib>		// std::srand, std::rand
#include <ctime>		// std::time
#include <numeric>		// std::accumulate
#include <vector>		// std::vector

// average
double average(std::vector<double> numbers)
{
	double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
	double result = sum / numbers.size();
	return(result);
}

// standard deviation
double stdev(std::vector<double> numbers)
{
	double mean = average(numbers);
	double minus, squared;
	std::vector<double> minus_squared = {};
	int I = numbers.size();
	for (int i = 0; i < I; i++)
	{
		minus = numbers[i] - mean;
		squared = std::pow(minus, 2);
		minus_squared.push_back(squared);
	}
	double sum = std::accumulate(minus_squared.begin(), minus_squared.end(), 0.0);
	double N = I;
	double result = std::sqrt((1 / N) * sum);
	return(result);
}

// speedup formula
double speedup(double time_length_s, double time_length_p)
{
	double T1 = time_length_s;
	double Tj = time_length_p;
	double S = T1 / Tj;
	return(S);
}

// efficiency formula [3]
double efficiency(double S, int C)
{
	double p = C;
	double E = S / p;
	return(E);
}

// random scale divider
std::vector<double> rand_divider(int divisions)
{
	std::vector<int> i_result;
	std::vector<double> result;
	int r, s, t = 0;
	if (divisions < 2 || divisions > 10) // divisions must be between 2 and 10
		result.push_back(-1);
	else
	{
		for (int i = 0; i < divisions; i++)
		{
			std::srand(time(0)); // initiate random seed
			if (i == 0) // if first division
				r = std::rand() % 50 + 2; // then load random number in range 2 to 50
			else if (i == divisions - 1) // if last division
				i_result.push_back(100); // save remaining distribution
			else
				r = std::rand() % (s / 2) + 2; // load random number in range 2 to remaining distribution
			if (i != divisions - 1)
			{
				t += r; // increment division mark
				i_result.push_back(t); // save to result
				s = 100 - t; // remaining distribution
			}
		}
		double v, w;
		for (int i = 0; i < divisions; i++)
		{
			v = i_result[i];
			w = v / 100;
			result.push_back(w);
		}
	}
	return(result);
}