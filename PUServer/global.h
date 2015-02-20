#ifndef __global_h_
#define __global_h_

#include <iostream>
#include <sstream>

#include "definitions.h"
#include "enums.h"
#include "tools.h"

using namespace std;

class Global
{
private:
	static string titleBarString;
	static int connectedPlayers;
public:

	static void setTitle(string newTitle)	
	{
		stringstream newTitleMsg;
		newTitleMsg << PUSERV_NAME << " - Operation RageQuit " << newTitle;

		titleBarString = newTitleMsg.str();

#ifdef WIN32
		string title = "title ";
		title += titleBarString;
		system(title.c_str());
#else
		char esc_start[] = { 0x1b, ']', '0', ';', 0 };
		char esc_end[] = { 0x07, 0 };
		cout << esc_start << titleBarString << esc_end;
#endif	
	}

	static void updatePlayers(int playerCount)
	{
		connectedPlayers = playerCount;

		stringstream newTitleMsg;
		newTitleMsg << titleBarString << " - Players: " << connectedPlayers;

#ifdef WIN32
		string title = "title ";
		title += newTitleMsg.str();
		system(title.c_str());
#else
		char esc_start[] = { 0x1b, ']', '0', ';', 0 };
		char esc_end[] = { 0x07, 0 };
		cout << esc_start << newTitleMsg.str() << esc_end;
#endif	
	}

	static void PauseCmd()
	{
		printf("*** Press 'Enter' to continue... ***");

		getchar();
		while (getchar() != '\n');
	}

	static vector<string> StringSplit(string str, string delim)
	{
		unsigned int cutAt;
		vector<string> results;
		while( (cutAt = str.find_first_of(delim)) != str.npos )
		{
			if(cutAt > 0)
			{
				results.push_back(str.substr(0,cutAt));
			}

			str = str.substr(cutAt+1);
		}

		if(str.length() > 0)
		{
			results.push_back(str);
		}

		return results;
	}
};

#endif
