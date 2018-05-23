#include <chrono>		// std::chrono

std::chrono::time_point<std::chrono::system_clock> timer_initial, timer_final; // initiate timer markers

// function timer
double timer(bool begin)
{
	double result;
	if (begin == true) // start the timer?
	{
		timer_initial = std::chrono::system_clock::now(); // start timer
		result = 0;
	}
	else // end the timer?
	{
		timer_final = std::chrono::system_clock::now(); // end timer
		double elapsed_time = std::chrono::duration<double, std::milli>(timer_final - timer_initial).count(); // time length
		result = elapsed_time; // save time length in milliseconds
	}
	return(result);
}