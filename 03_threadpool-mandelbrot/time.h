#include <chrono>
#include <iostream>
using namespace std;

/* References
[1]: Date and Time Utilities _ http://en.cppreference.com/w/cpp/chrono
*/

chrono::time_point<chrono::system_clock> timer_initial, timer_final; // initiate timer markers

// function timer [1]
double timer(bool begin)
{
	double result;
	if (begin == true) // start the timer?
	{
		timer_initial = chrono::system_clock::now(); // start timer
		result = 0;
	}
	else // end the timer?
	{
		timer_final = chrono::system_clock::now(); // end timer
		double elapsed_time = chrono::duration<double, milli>(timer_final - timer_initial).count(); // time length
		result = elapsed_time; // save time length in milliseconds
	}
	return(result);
}