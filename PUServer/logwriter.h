#ifndef __logwriter_h_
#define __logwriter_h_

#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

class LogWriter
{
public: 
	LogWriter();
	~LogWriter();
	
	bool _canWrite;

	fstream myFile;

	void WriteLine(std::string r_s_);

protected:
	void OpenLog();
};

#endif