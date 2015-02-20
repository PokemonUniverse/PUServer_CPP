#ifndef __serverthread_h_
#define __serverthread_h_

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>
#include <ctime> 

#include "game.h"
#include "configmanager.h"
#include "filesystem.h"
#include "logwriter.h"
#include "database.h"

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

extern ConfigManager g_config;
extern Database g_db;
extern LogWriter g_log;
extern Game g_game;

class NpcManager;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

enum LoginStatus
{
	STATUS_IDLE = 0,
	STATUS_WRONGACCOUNT = 1,
	STATUS_SERVERERROR = 2,
	STATUS_DATABASEERROR = 3,
	STATUS_LOGGEDIN = 4,
	STATUS_READY = 5,
	STATUS_CHARBANNED = 6,
	STATUS_SERVERCLOSED = 7,
	STATUS_WRONGVERSION = 8,
};

class ServerThread
{
public:
	Database *database;

	ServerThread();
	~ServerThread();

	bool StartThread();
	void StopThread();

	void Run();
	void TimeOutCheck();
	void session(socket_ptr sock);

private:
	bool _requestStopped;

	NpcManager* m_NpcManager;

	boost::asio::io_service conn_io_service;
	boost::asio::ip::tcp::acceptor* m_acceptor;

	boost::thread myThread;
	
	void loadEncounterSections();
	void closeListenSocket();
	void loadPokeCenters();
	void loadTowns();
};

#endif