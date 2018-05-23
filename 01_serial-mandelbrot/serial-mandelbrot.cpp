// 01/14/2017 - CS 3100 - Meine, Joel
// Serial MandelBrot

/* References
[1]: Mandelbrot Set _ https://en.wikipedia.org/wiki/Mandelbrot_set
[2]: C++ Make an Image _ https://www.youtube.com/watch?v=fbH005SzEMc
*/

#include "math.h"
#include "time.h"

#include <fstream>
#include <iostream>
using namespace std;

int scale; // mandelbrot-to-pixel scalar
int samples; // number of function execution samples

int width, height; // image width and height
const int max_iteration = 256; // max number of colors
const double x_min = -2.5, x_max = 1, y_min = -1, y_max = 1; // mandelbrot axis
const int bailout = 2 * 2; // mandelbrot bailout limit
double x_range = x_max - x_min, y_range = y_max - y_min; // mandelbrot axis ranges
double x_unit, y_unit; // pixel-to-mandelbrot point unit
vector<int> m_colors = {}; // mandelbrot point colors of image
double time_length; // timer sample
vector<double> times = {}; // timer samples

// mandelbrot point color [1]
int m_color(double Px, double Py)
{
	double x0 = Px * x_unit - abs(x_min), y0 = Py * y_unit - abs(y_min); // pixel-to-mandelbrot scaled+shifted point
	double x = 0, y = 0; // initalize madelbrot point
	int iteration = 0; // initialize iteration
	double temp;
	// escape time algorithm
	while (x*x + y*y < bailout && iteration < max_iteration)
	{
		temp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = temp;
		iteration++;
	}
	return(iteration); // return color of mandelbrot point
}

// mandelbrot image [1]
void m_image()
{
	for (int y = 0; y < height; y++) // every pixel of height
	{
		for (int x = 0; x < width; x++) // every pixel of width
		{
			if (y == 0 && x == 0) // is this the first pixel of the image?
			{
				cout << "start timer" << endl;
				cout << "creating mandelbrot point colors... ";
				time_length = timer(true); // if yes, then start timer
			}
			m_colors.push_back(m_color(x, y)); // save mandelbrot point color
			if (y == height - 1 && x == width - 1) // is this the last pixel of the image?
			{
				cout << "DONE" << endl;
				time_length = timer(false); // if yes, then end timer
				cout << "end timer" << endl;
				cout << "timer = " << time_length << " milliseconds" << endl;
			}
		}
	}
}

// write image [2]
void w_image()
{
	cout << "writing mandelbrot image... ";
	ofstream img("m_image.ppm");
	img << "P3" << endl;
	img << width << " " << height << endl;
	img << max_iteration - 1 << endl; // number of colors
	int I = m_colors.size();
	int r, g, b; // initialize color channels
	for (int i = 0; i < I; i++) // every pixel in image
	{
		r = m_colors[i]; // set red color channel
		g = m_colors[i]; // set green color channel
		b = m_colors[i]; // set blue color channel
		img << r << " " << g << " " << b << endl;
	}
	cout << "DONE" << "\n" << endl;
}

int main()
{
	// ask user for image width input
	cout << "image width, pixels: ";
	cin >> width; cout << endl;
	double scale = width / x_range;
	height = y_range * scale;
	x_unit = x_range / width; y_unit = y_range / height;
	// ask user for number of image samples
	cout << "image samples, number of: ";
	cin >> samples; cout << endl;

	int sample = 0;
	// create madelbrot images
	while (sample < samples)
	{
		cout << "sample " << sample+1 << " of " << samples << endl;
		m_image(); // create the mandelbrot image
		times.push_back(time_length); // save time to create image
		w_image(); // write the mandelbrot image
		sample++; // increment to next sample
	}
	cout << "average = " << average(times) << "\n" << endl;
	cout << "standard deviation = " << stdev(times) << "\n" << endl;

	return 0;
}