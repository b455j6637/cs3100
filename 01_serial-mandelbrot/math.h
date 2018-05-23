#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>
using namespace std;

/* References
[1]: How to Find the Mean _ https://www.mathsisfun.com/mean.html
[2]: Standard Deviation Formulas _ https://www.mathsisfun.com/data/standard-deviation-formulas.html
[ ]: Calculate mean and standard deviation... _ http://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
*/

// average [1]
double average(vector<double> numbers)
{
	double sum = accumulate(numbers.begin(), numbers.end(), 0.0);
	double result = sum / numbers.size();
	return(result);
}

// standard deviation [2]
double stdev(vector<double> numbers)
{
	double mean = average(numbers);
	double minus, squared;
	vector<double> minus_squared = {};
	int I = numbers.size();
	for (int i = 0; i < I; i++)
	{
		minus = numbers[i] - mean;
		squared = pow(minus, 2);
		minus_squared.push_back(squared);
	}
	double sum = accumulate(minus_squared.begin(), minus_squared.end(), 0.0);
	double N = I;
	double result = sqrt((1 / N) * sum);
	return(result);
}