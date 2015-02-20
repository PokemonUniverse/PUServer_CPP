#include "server.h"

#include <boost/thread/shared_mutex.hpp>

#include "game.h"
#include "database.h"
#include "logger.h"
#include "tools.h"
#include "connection.h"
#include "ioplayer.h"
#include "player.h"

#include "receiveloginmessage.h"
#include "sendloginmessage.h"

bool Server::m_requestStopped = false;

extern Game g_game;

Server::~Server() 
{
	m_conn_io_service.stop();
}

bool Server::startServer()
{
	m_requestStopped = false;

	// Start time-out thread

	// Start login thread
	m_loginThread = boost::thread(boost::bind(&Server::loginThread, this));

	Logger::consoleOutput(COUTPUT_MESSAGE, "Server data loaded and now accepting new connections.");
	m_loginThread.join();

	return true;
}

bool Server::stopServer()
{
	return true;
}

void Server::loginThread()
{
	// Setup connection endpoint and port
	int			serverPort	= atoi(g_config.pString("SERVER_PORT").c_str());
	std::string serverIP	= g_config.pString("SERVER_IP");

	tcp::endpoint localEndPoint(tcp::v4(), serverPort);
	m_acceptor = new boost::asio::ip::tcp::acceptor(m_conn_io_service, localEndPoint);
	if(!m_acceptor) {
		Logger::consoleOutput(COUTPUT_ERROR, "Failed to setup network acceptor.");
		return;
	}

	m_conn_io_service.run();

	// All done - into the loop - fetch connecting players
	while(!Server::m_requestStopped)
	{
		boost::system::error_code error;

		// Receive new connection
		socket_ptr socketptr(new SocketPtr(m_conn_io_service));
		m_acceptor->accept(socketptr->m_socket, error);
		if(error)
		{
			Logger::consoleOutput(COUTPUT_ERROR, error.message());

			continue;
		}

		// Read data
		packet_ptr packet(new Packet());
		boost::asio::read(socketptr->m_socket, boost::asio::buffer(packet->getBuffer(), 2),  boost::asio::transfer_at_least(2), error);
		if(error && boost::asio::error::would_block != error)
		{
			Logger::consoleOutput(COUTPUT_ERROR, "Client acceptor | " + error.message());
			continue;
		}

		boost::asio::read(socketptr->m_socket, boost::asio::buffer(packet->getDataBuffer(), packet->getHeader()), boost::asio::transfer_at_least(packet->getHeader()), error);
		ReceiveLoginMessage recvLogin(packet);
		if(recvLogin.getHeader() != HEADER_LOGIN) {
			continue;
		}

		SendLoginMessage sendLogin;
		std::string username = asLowerCaseString(recvLogin.getUsername());
		std::string password = recvLogin.getPassword();

		Database* db = Database::getFactory();
		DBQuery query;
		query << boost::str(boost::format("SELECT idplayer, name, password, password_salt FROM player WHERE name='%s'") % username);

		DBResult* result;
		if((result = db->storeQuery(query.str())) == NULL) {
			sendLogin.addStatus(STATUS_WRONGACCOUNT);
		}
		else
		{
			if(g_game.getGameState() == GAME_STATE_CLOSING) {
				sendLogin.addStatus(STATUS_SERVERCLOSING);
			}
			else if(g_game.getGameState() == GAME_STATE_CLOSED) {
				sendLogin.addStatus(STATUS_SERVERCLOSED);
			}
			else if(recvLogin.getClientVersion() != PUSERV_CLIENT_VERSION) {
				sendLogin.addStatus(STATUS_WRONGVERSION);
			}				
			else if(!passwordTest(password, result->getDataString("password") ) ) {
				sendLogin.addStatus(STATUS_WRONGACCOUNT);
			}
			/*else if(result->getDataInt("User_GroupID") == 0) { // Banned
							sendLogin.addStatus(STATUS_CHARBANNED);
			}*/
			else // Player exists, now we check if this player has an abandoned object
			{
				Player* player;
				Connection* connection;
				if((player = g_game.getPlayerByID(result->getDataInt("idplayer")) ) != NULL)
				{
					/*
					* The connecting player has just disconnected, so the pointer is still active
					* Try to rejoin this connection with the old player object
					*/
					connection = new Connection(socketptr, player);
					player->assignConnection(connection);

					sendLogin.addStatus(STATUS_READY);
				}
				else
				{
					username = result->getDataString("name");
					player = new Player(username);
					if(!IOPlayer::instance()->loadPlayer(player, username))
					{
						sendLogin.addStatus(STATUS_FAILPROFILELOAD);
						delete player;
					}
					else
					{
						connection = new Connection(socketptr, player);
						player->assignConnection(connection);

						g_game.addCreature(player);
						Logger::consoleOutput(COUTPUT_MESSAGE, boost::str(boost::format("Player (%1%) connected - %2%") % player->getName() % player->getGUID()));

						sendLogin.addStatus(STATUS_READY);
					}
				}
			}
		}

		db->freeResult(result);

		// Send packet
		sendLogin.send(socketptr->m_socket);
	}
}

void Server::timeoutThread()
{
	/*
	* This threads checks if players are idle for X amount of time
	* if they are they'll receive a message and if they continue to be idle
	* their connection will be closed.
	*/
	Creature* c = NULL;
	while(!Server::m_requestStopped)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(10));

		boost::upgrade_lock<boost::shared_mutex> lock(g_game.m_creatureMutex);
		foreach(creatureList_t::value_type it, g_game.m_creatureList)
		{
			if((c = it.second) == NULL) continue;

			// If creature needs to be removed from the list, get unique access
			// boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
		}
	}
}

void Server::closeListenSocket()
{
	if(m_acceptor)
	{
		if(m_acceptor->is_open())
		{
			boost::system::error_code error;
			m_acceptor->close(error);
		}

		delete m_acceptor;
		m_acceptor = NULL;
	}
}