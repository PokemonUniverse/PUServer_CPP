#include <iostream>
#include <ctime>

#include "global.h"
#include "serverthread.h"
#include "game.h"
#include "filesystem.h"
#include "pokecode.h"
#include "configmanager.h"
#include "chat.h"
#include "gametext.h"
#include "commands.h"

ServerThread gameThread;
ConfigManager g_config;
LogWriter g_log;
Database g_db;
Game g_game;
Chat g_chat;
GameText g_gameText;
Commands commands;

using namespace std;

string Global::titleBarString = "";
int Global::connectedPlayers = 0;

#ifdef __BUGTRAP__
#include <eh.h>
#include "BugTrap.h"
static void SetupExceptionHandler()
{
	BT_SetAppName((LPCTSTR)"PUServer");
	BT_SetFlags(BTF_DETAILEDMODE | BTF_RESTARTAPP);
	BT_SetSupportServer((LPCTSTR)"94.75.231.83", 6660);
	BT_SetActivityType(BTA_SENDREPORT);

	BT_InstallSehFilter();
}
#endif

int main()
{
#ifdef __BUGTRAP__
	SetupExceptionHandler();
	BT_SetTerminate();
	std::cout << "BugTrap Enabled..." << endl;
#endif // __BUGTRAP__
	
	Global::setTitle("");

	cout << "*********************************************" << endl;
	cout << "**         Pokemon Universe Server         **" << endl;
	cout << "**           Operation  RageQuit           **" << endl;
	cout << "*********************************************" << endl;
	cout << "**       http://pokemon-universe.com       **" << endl;
	cout << "*********************************************" << endl << endl;

	srand(time(0));
	if(!gameThread.StartThread())
	{
		cout << "An error occured while loading the server..." << std::endl;
		Global::PauseCmd();
	}

	cout << "\n* END OF APPLICATION, closing window, please stand by . . .";
	
	return 0;
}