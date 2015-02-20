#ifndef __SERVER_H_
#define __SERVER_H_

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <ctime> 

#include "configmanager.h"
#include "game.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

extern Game g_game;
extern ConfigManager g_config;

class Server
{
public:
	~Server();

	static Server* instance()
	{
		static Server instance;
		return &instance;
	}

	bool startServer();
	bool stopServer();

protected:
	void loginThread();
	void timeoutThread();

	void closeListenSocket();
	
private:
	Server() : m_conn_io_service() {};

	boost::asio::io_service m_conn_io_service;
	boost::asio::ip::tcp::acceptor* m_acceptor;

	boost::thread m_loginThread;
	boost::thread m_timeoutThread;

	static bool m_requestStopped;
};
#endif