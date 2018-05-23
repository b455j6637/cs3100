// 02/18/2017 - CS 3100 - Meine, Joel
// Shell - Part 1

/*
Credits
~ Instructor: Sundberg, Kenneth
~ Lab Tutors: Kiefer, Jack & Nelson, Philip
*/

#include <cmath> // std::ceil
#include <cstring> // strcpy
#include <iostream> // std::cout, std::cin
#include <sstream> // std::stringstream
#include <string> // std::string, stoi
#include <sys/wait.h> // waitpid()
#include "time.h" // timer()
#include <unistd.h> // fork(), execvp()
#include <vector> // std::vector

double time_length = -1;
std::vector<double> times = {};

std::vector<std::string> args = {}; // a command in executable format
std::vector<const char *> argt; // a command in exectuable format for execvp()
void shell()
{
	auto pid = fork();
	if (pid < 0)
	{
		perror("ERROR! >> fork()");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) // child process
	{
		char ** argr = new char *[args.size() + 1];
		for (int i = 0; i < args.size(); i++)
		{
			argr[i] = new char[args[i].size() + 1];
			strcpy(argr[i], args[i].c_str());
		}
		argr[args.size()] = NULL;
		char * const *argu = argr;
		execvp(argu[0], argu);
		perror("ERROR! >> execvp()");
		exit(EXIT_FAILURE);
	}
	else // parent process
	{
		int status;
		time_length = timer(true); // start timer
		waitpid(pid, &status, 0);
		time_length = timer(false); // end timer
		times.push_back(time_length); // save time
		if (status == EXIT_FAILURE)
			perror("ERROR! >> wait()");
	}
}

void ptime() // child process time length of execution
{
	double time = 0;
	for (int i = 0; i < times.size(); i++)
		time += times[i];

	double seconds = time * 0.000001; // convert microseconds to seconds
	int intS = (long)seconds; // isolate integer portion of seconds
	double fraS = seconds - intS; // isolate fractional portion of seconds

	double milliseconds = fraS * 1000; // convert seconds to milliseconds
	int intM = (long)milliseconds; // isolate integer portion of milliseconds
	double fraM = milliseconds - intM; // isolate fractional portion of milliseconds

	double microseconds = fraM * 1000; // convert milliseconds to microseconds
	microseconds = std::round(microseconds); // round the microseconds value
	int intU = (long)microseconds; // isolate integer portion of microseconds
	double fraU = microseconds - intU; // isolate fractional portion of microseconds

	std::cout << "Time spent executing child processes: " << intS << " seconds " << intM << " milliseconds and " << intU << " microseconds" << std::endl;
}

std::vector<std::string> cmds = {}; // command history in non-executable format
std::vector<std::vector<std::string>> argv = {}; // command history in executable format
void history(std::vector<std::string> h) // previous commands executed in shell terminal
{
	if (h[0] == "history" && h.size() == 1) // print all previous commands executed
	{ // only "history" was typed by user
		for (int i = 0; i < cmds.size(); i++)
			std::cout << "#" << i + 1 << "  " << cmds[i] << std::endl;
	}
	else if (h[1] == "^" && h.size() == 3) // execute the specified previous command
	{ // only "history ^ ##" was typed by user
		int num = stoi(h[2], nullptr, 10);
		args = argv[num - 1];
	}
	else
		std::cout << "ERROR! >> history()" << std::endl;
}

int main(void)
{
	int run = 1;
	while (run == 1)
	{
		args.clear(); // clear last executable command
		std::string cmd; // initialize command line input
		std::cout << "[cmd]: "; // print shell command line prompt
		std::getline(std::cin, cmd); // save the command line input
		cmds.push_back(cmd); // save the command line input to history, non-executable
		std::stringstream command(cmd); // begin parsing of command line input
		std::string tmp; // intialize temporary string
		while (command >> tmp) args.push_back(tmp); // separate command from arguments
		argv.push_back(args); // save the command line input to history, executable

		if (!args.empty())
		{
			if (args[0] == "ptime" && args.size() == 1) // user has entered "ptime" command
			{
				ptime();
				args.clear();
			}
			else if (args[0] == "history" && args.size() == 1) // user has entered "history" command
			{
				history(args);
				args.clear();
			}
			else if (args[0] == "exit" && args.size() == 1) // user has entered "exit" command
				run = 0;
			else
			{
				if (args[0] == "history" && args.size() == 3) // user has entered "history ^ ##" command
					history(args); // run command specified in history

				if (args[0] == "ptime" && args.size() == 1) // user executes "ptime" from history
				{
					ptime();
					args.clear();
				}
				else if (args[0] == "history" && args.size() == 1) // user executes "history" from history
				{
					history(args);
					args.clear();
				}
				else if (args[0] == "history" && args[1] == "^")
					std::cout << "ERROR! >> Must call command in history directly." << std::endl;
				else
					shell(); // user executes non-custom command
			}
		}
		else
		{
			std::cout << "ERROR! >> Must enter a command." << std::endl;
		}
	}
	return 0;
}