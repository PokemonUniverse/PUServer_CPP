#include "logwriter.h"

LogWriter::LogWriter()
{
	_canWrite = false;
}

LogWriter::~LogWriter()
{
	if(_canWrite)
		myFile.close();
}

void LogWriter::WriteLine(std::string r_s_)
{
	OpenLog();

	if(LogWriter::_canWrite)
	{
		time_t now = time(0);
		tm* localtm = localtime(&now);
		
		char logstr[64] = "00_00_0000\0";
		sprintf(logstr, "[%02d:%02d:%02d] ", localtm->tm_hour, localtm->tm_min, localtm->tm_sec);	

		myFile << logstr << r_s_.c_str();

		myFile.close();
	}
}

void LogWriter::OpenLog()
{	
	try
	{
		char logstr[64] = "00_00_0000\0";

		time_t now = time(0);
		tm* localtm = localtime(&now);
		sprintf(logstr, "%02d_%02d_%02d", localtm->tm_mday, (localtm->tm_mon + 1), (localtm->tm_year + 1900));
		
		string filename;
		filename = (string)logstr;
		
		std::stringstream LogName;
		LogName << "Data/Logs/Log_" << filename << ".txt";

		myFile.open(LogName.str().c_str(), ios::out | ios::app);

		LogWriter::_canWrite = true;
	}
	catch(...)
	{
		LogWriter::_canWrite = false;
	}
}