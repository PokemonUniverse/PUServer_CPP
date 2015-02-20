#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "definitions.h"

enum ConsoleOutput_t
{
	COUTPUT_MESSAGE = 0,
	COUTPUT_ERROR	= 1,
	COUTPUT_WARNING = 2,
	COUTPUT_DEBUG	= 3,
	COUTPUT_LOAD	= 4,
	COUTPUT_SQL		= 5,
};

class Logger
{
public:
	~Logger() {};

	static void consoleOutput(ConsoleOutput_t type, std::string message, bool writeLog = false)
	{
		switch(type)
		{
		case COUTPUT_MESSAGE:
			message = "[Message] " + message;
			break;

		case COUTPUT_ERROR:
			message = "[Error] " + message;
			break;

		case COUTPUT_WARNING:
			message = "[Warning] " + message;
			break;

		case COUTPUT_DEBUG:
			message = "[Debug] " + message;
			break;

		case COUTPUT_LOAD:
			message = "[Loading] " + message;
			break;

		case COUTPUT_SQL:
			message = "[Query] " + message;
			break;

		default:
			break;
		}

		consoleOutput(message, writeLog);
	}

	static void consoleOutput(std::string message, bool writeLog = false)
	{
		std::cout << message << std::endl;

		if(writeLog)
		{
			//TODO: Write to log here
		}
	}

private:
	Logger() {};
};

#endif