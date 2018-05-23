// 02/25/2017 - CS 3100 - Meine, Joel
// Shell - Part 2

/*
Credits
~ Instructor: Sundberg, Kenneth
~ Lab Tutors: Kiefer, Jack & Nelson, Philip
*/

#include <cmath> // std::ceil
#include <cstring> // strcpy
#include <fcntl.h>
#include <iostream> // std::cout, std::cin
#include <sstream> // std::stringstream
#include <string> // std::string, stoi
#include <sys/wait.h> // waitpid()
#include <sys/stat.h>
#include <sys/types.h>
#include "time.h" // timer()
#include <unistd.h> // fork(), execvp(), dup2(), pipe()
#include <vector> // std::vector

double time_length = -1;
std::vector<double> times = {};

std::vector<std::string> args = {}; // a command in parsed format

void printArgs()
{
	std::cout << "args = ";
	for (int i = 0; i < args.size(); i++)
		std::cout << args[i] << " ";
	std::cout << std::endl;
}

// convert command from parsed format to executable format
char * const *arge(std::vector<std::string> argp)
{
	char ** argr = new char *[argp.size() + 1];
	for (int i = 0; i < argp.size(); i++)
	{
		argr[i] = new char[argp[i].size() + 1];
		strcpy(argr[i], argp[i].c_str());
	}
	argr[argp.size()] = NULL;
	return(argr);
}

void shell_v1()
{
	auto pid = fork();
	if (pid < 0)
	{
		perror("ERROR! >> fork()");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) // child process
	{
		char * const *argu = arge(args); 
		execvp(argu[0], argu);
		perror("ERROR! >> execvp()");
		exit(EXIT_FAILURE);
	}
	else // parent process
	{
		int status = 0;
		time_length = timer(true); // start timer
		waitpid(pid, &status, 0);
		time_length = timer(false); // end timer
		times.push_back(time_length); // save time
		if (status == EXIT_FAILURE)
			perror("ERROR! >> wait()");
	}
}

struct Cmd {
	std::vector<std::string> args_left; // parsed command left of operator
	std::vector<std::string> args_right; // parsed command right of operator
	bool isFileName; // if true, output to file; else, output to terminal
	bool isInFile; // if true, file is input; else, file is output
} myCmd;
std::vector<Cmd> Cmds;

std::vector<std::vector<std::string>> Args; // commands in parsed format
std::string InFile; // command receives input from file
std::string OutFile; // command sends output to file

void printCmds()
{
	for (int i = 0; i < Cmds.size(); i++)
	{
		std::cout << "Cmd #" << i+1 << " of " << Cmds.size() << std::endl;
		std::cout << "args_left = ";
		for (int j = 0; j < Cmds[i].args_left.size(); j++)
			std::cout << Cmds[i].args_left[j] << " ";
		std::cout << std::endl;
		std::cout << "args_right = ";
		for (int j = 0; j < Cmds[i].args_right.size(); j++)
			std::cout << Cmds[i].args_right[j] << " ";
		std::cout << std::endl;
		std::cout << "output to ";
		if (Cmds[i].isFileName == true)
		{
			std::cout << "file = ";
			if (Cmds[i].isInFile == true)
				std::cout << "IN";
			else
				std::cout << "OUT";
		}
		else
			std::cout << "terminal";
		std::cout << "\n" << std::endl;
	}
}

// build the commands chain in parsed format
std::vector<int> indexes = {};
void buildCmds()
{
	int j = 0; int k = 0; int l = 0; int m = 0;
	for (int i = 0; i < indexes.size(); i++)
	{
		j = indexes[i]; // load location of current operator character
		if (i != indexes.size()-1)
			k = indexes[i+1]; // load location of next operator character
		else
			k = args.size();
		l; // set left marker
		m = j+1; // set right marker
		for (l; l < j; l++)
			myCmd.args_left.push_back(args[l]); // save left arg
		for (m; m < k; m++)
			myCmd.args_right.push_back(args[m]); // save right arg
		if (args[j] == "<" || args[j] == ">") // executable for file
		{
			myCmd.isFileName = true;
			if (args[j] == "<")
				myCmd.isInFile = true;
			else if (args[j] == ">")
				myCmd.isInFile = false;
			else
				perror("ERROR! >> Invalid operator detected.");
		}
		else if (args[j] == "|") // executable for terminal
			myCmd.isFileName = false;
		else
			perror("ERROR! >> Invalid character detected.");
		Cmds.push_back(myCmd); // save to commands queue
		myCmd = {};
		l = j+1;
	}
	indexes = {};
}

int sizeArgs = 0;
void buildArgs()
{
	bool isFileError = false;
	for (int c = 0; c < Cmds.size(); c++)
	{
		if (Cmds[c].isFileName == true) // file
		{
			std::cerr << "file detected" << std::endl;
			std::cerr << "isFileError = ";
			if (isFileError == 1)
				std::cerr << "true";
			else
				std::cerr << "false";
			std::cerr << std::endl;
			if (isFileError == false)
			{
				std::cerr << "isFileError == false" << std::endl;
				std::cerr << "Args.push_back >> ";
				for (int j = 0; j < Cmds[c].args_left.size(); j++)
					std::cerr << Cmds[c].args_left[j] << " ";
				std::cerr << std::endl;
				Args.push_back(Cmds[c].args_left);
				sizeArgs++;
				if (Cmds[c].isInFile == true) // input file
				{
					InFile = Cmds[c].args_right[0];
					std::cerr << "InFile = " << InFile << std::endl;
				}
				else // output file
				{
					OutFile = Cmds[c].args_right[0];
					std::cerr << "OutFile = " << OutFile << std::endl;
				}
				isFileError = true;
			}
		}
		else // terminal
		{
			std::cerr << "terminal detected" << std::endl;
			if (Cmds.size() == 1)
			{
				std::cerr << "<Args.push_back >> ";
				for (int j = 0; j < Cmds[c].args_left.size(); j++)
					std::cerr << Cmds[c].args_left[j] << " ";
				std::cerr << std::endl;
				Args.push_back(Cmds[c].args_left);
				sizeArgs++;
				std::cerr << ">Args.push_back >> ";
				for (int j = 0; j < Cmds[c].args_right.size(); j++)
					std::cerr << Cmds[c].args_right[j] << " ";
				std::cerr << std::endl;
				Args.push_back(Cmds[c].args_right);
				sizeArgs++;
			}
			else
			{
				if (isFileError == false)
				{
					std::cerr << "+Args.push_back >> ";
					for (int j = 0; j < Cmds[c].args_left.size(); j++)
						std::cerr << Cmds[c].args_left[j] << " ";
					std::cerr << std::endl;
					Args.push_back(Cmds[c].args_left);
					sizeArgs++;
					if (c == Cmds.size()-1)
					{
						std::cerr << "-Args.push_back >> ";
						for (int j = 0; j < Cmds[c].args_right.size(); j++)
							std::cerr << Cmds[c].args_right[j] << " ";
						std::cerr << std::endl;
						Args.push_back(Cmds[c].args_right);
						sizeArgs++;
					}
				}
				else
				{
					std::cerr << "*Args.push_back >> ";
					for (int j = 0; j < Cmds[c].args_right.size(); j++)
						std::cerr << Cmds[c].args_right[j] << " ";
					std::cerr << std::endl;
					Args.push_back(Cmds[c].args_right);
					sizeArgs++;
					if (c == Cmds.size()-2)
					{
						std::cerr << "last Cmd detected" << std::endl;
						if (Cmds[c+1].isFileName == true)
						{
							std::cerr << "set isFileError to false" << std::endl;
							Args.pop_back();
							sizeArgs--;
							isFileError = false;
						}
					}
				}
			}
		}
	}
}

void printArgs2()
{
	std::cout << "Args = " << std::endl;
	for (int i = 0; i < sizeArgs; i++)
	{
		std::cout << "Cmd #" << i+1 << " of " << sizeArgs << std::endl;
		for (int j = 0; j < Args[i].size(); j++)
		{
			std::cout << Args[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "endArgs" << std::endl;
	std::cout << "InFile = " << InFile << std::endl;
	std::cout << "OutFile = " << OutFile << std::endl;
}

bool isIN = false;
void shell_v2()
{
	std::cerr << "Starting shell_v2()..." << std::endl;
	std::cin.get();

	int p[2][2]; // intialize array container for two pipes
	pipe(p[0]); pipe(p[1]); // initialize two pipes
	int current_in = 0;
	int current_out = 1;
	int Read = 0;
	int Write = 1;

	int file;
	for (int i = 0; i < sizeArgs; i++)
	{
		std::cerr << "#" << i+1 << " Cmd of " << sizeArgs << " Cmds" << std::endl;
		auto pid = fork();
		if (pid < 0)
		{
			perror("ERROR! >> fork()");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) // child process
		{
			std::cerr << "Into child process..." << std::endl;
			char * const*argu;
			if (i == 0) // first Cmd
			{
				std::cerr << "first Cmd..." << std::endl;
				std::cerr << "Args[" << i << "] = ";
				for (int j = 0; j < Args[i].size(); j++)
					std::cerr << Args[i][j] << " ";
				std::cerr << std::endl;
				argu = arge(Args[i]);
				if (sizeArgs == 1)
				{
					std::cerr << "sizeArgs = 1" << std::endl;
					if (InFile.empty() == false)
					{
						std::cerr << "InFile" << std::endl;
						file = open(InFile.c_str(), O_RDWR, S_IWUSR | S_IRUSR);
						dup2(file, STDIN_FILENO);
						close(file);
					}
					else if (OutFile.empty() == false)
					{
						std::cerr << "OutFile" << std::endl;
						file = open(OutFile.c_str(), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
						dup2(file, STDOUT_FILENO);
						close(file);
					}
				}
				else
				{
					std::cerr << "sizeArgs != 1" << std::endl;
					if (InFile.empty() == false)
					{
						std::cerr << "*InFile" << std::endl;
						file = open(InFile.c_str(), O_RDWR, S_IWUSR | S_IRUSR);
						dup2(file, STDIN_FILENO);
						close(file);
					}
					dup2(p[current_out][Write], STDOUT_FILENO);
					close(p[current_out][Write]);
				}
			}
			else if (i == sizeArgs - 1) // last
			{
				std::cerr << "last Cmd..." << std::endl;
				std::cerr << "Args[" << i << "] = ";
				for (int j = 0; j < Args[i].size(); j++)
					std::cerr << Args[i][j] << " ";
				std::cerr << std::endl;
				argu = arge(Args[i]);
				if (OutFile.empty() == false) // use file
				{
					std::cerr << "using file... ";
					std::cerr << OutFile << std::endl;
					file = open(OutFile.c_str(), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
					dup2(file, STDOUT_FILENO);
					close(file);
				}
				dup2(p[current_in][Read], STDIN_FILENO);
				close(p[current_in][Read]);
			}
			else
			{
				std::cerr << "middle Cmd..." << std::endl;
				std::cerr << "Args[" << i << "] = ";
				for (int j = 0; j < Args[i].size(); j++)
					std::cerr << Args[i][j] << " ";
				std::cerr << std::endl;
				argu = arge(Args[i]);
				dup2(p[current_in][Read], STDIN_FILENO);
				dup2(p[current_out][Write], STDOUT_FILENO);
				close(p[current_in][Read]);
				close(p[current_out][Write]);
			}
			std::cerr << "...out of child process" << std::endl;
			std::cerr << "argu = ";
			for (int j = 0; j < Args[i].size(); j++)
				std::cerr << argu[j] << " ";
			std::cerr << std::endl;
			execvp(argu[0], argu);
			perror("ERROR! >> execvp()");
			exit(EXIT_FAILURE);
		}
		else // parent process
		{
			std::cerr << "Into parent process..." << std::endl;
			close(p[current_out][Write]); // close write of pipe
			int status = 0;
			time_length = timer(true); // start timer
			waitpid(pid, &status, 0);
			std::swap(current_in, current_out);
			time_length = timer(false); // end timer
			times.push_back(time_length); // save time
			std::cerr << "...out of parent process" << std::endl;
			if (status == EXIT_FAILURE)
				perror("ERROR! >> wait()");
		}
	}
	std::cerr << "...exiting shell_v2()" << std::endl;
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
		perror("ERROR! >> history()");
}

bool isRedirect()
{
	bool result = false;
	for (int i = 0; i < args.size(); i++)
	{
		if (args[i] == "<" || args[i] == ">" || args[i] == "|")
		{
			indexes.push_back(i);
			result = true;
		}
	}
	return(result);
}

int main()
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

		bool isIO = isRedirect();
		if (isIO == true)
		{
			InFile = "";
			OutFile = "";
			Args = {};
			sizeArgs = 0;
			Cmds = {};
			buildCmds();
			buildArgs();
		}

		if (args.empty() == true)
			perror("ERROR! >> User must enter a command.");
		else
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
					perror("ERROR! >> Must call command in history directly.");
				else
				{
					if (isIO == false)
						shell_v1(); // user executes non-custom command with single executables
					else
					{
						std::cerr << "Into shell_v2()..." << std::endl;
						printArgs();
						printCmds();
						printArgs2();
						std::cin.get();
						shell_v2(); // user executes non-custom command with multiple executables
						std::cerr << "out of shell_v2()..." << std::endl;
					}
				}
			}
		}
	}
	return 0;
}
