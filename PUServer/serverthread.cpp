#include "global.h"
#include "md5wrapper.h"

#include <boost/format.hpp>

#include "serverthread.h"
#include "npc.h"
#include "game.h"
#include "gametext.h"
#include "commands.h"

#ifdef __BUGTRAP__
#include <eh.h>
#include "BugTrap.h"
#endif

extern Game g_game;
extern GameText g_gameText;
extern Commands commands;

ServerThread::ServerThread() : conn_io_service()
{
	_requestStopped = false;
	database = new Database();
	m_NpcManager = new NpcManager();
}

ServerThread::~ServerThread()
{
	conn_io_service.stop();

	delete database;
}

bool ServerThread::StartThread()
{
	cout << "* Initializing server..." << std::endl;

	/////////////////////////////////////////////
	// Loading config file
	cout << " - Loading configuration file `server.conf`" << std::endl;
	if(!FileSystem::FileExists("data/server.config")) {
		cout << " [ERROR] Can not find the file: data/server.config" << std::endl;
		return false;
	}
	g_config.loadFile("data/server.config");	

	/////////////////////////////////////////////
	// Connecting to MySQL database
	cout << " - Connecting to MySQL database" << std::endl;

	if(!g_db.connectDB() || !database->connectDB()) {
		cout << " [ERROR] Could not connect to the database!!" << std::endl;
		return false;	
	}
	database->runQuery("UPDATE user SET User_Online='0' WHERE User_Online='1'");

	/////////////////////////////////////////////
	// Loading command xml
	cout << " - Loading commands xml" << std::endl;
	if(!commands.loadXml(g_config.pString("dataFolder")))
	{
		cout << " [ERROR] Erorr while loading commands.xml" << std::endl;
		return false;	
	}

	/////////////////////////////////////////////
	// Load all text stuff
	if(!g_gameText.loadText(g_config.pString("textfile")))
	{
		cout << " [ERROR] Could not load text file..." << std::endl;
		return false;
	}

	/////////////////////////////////////////////
	// Loading items
	cout << " - Loading items" << std::endl;
	if(!g_game.loadItems())
	{
		cout << " [ERROR] Could not load items." << std::endl;
		return false;
	}

	/////////////////////////////////////////////
	// Check if solution file exists
	cout << " - Check solution file existence" << std::endl;
	std::string solutionDir = g_config.pString("dataFolder") + "/";
	std::string outsideMap = solutionDir + g_config.pString("xmlWorldMap");
	if(!FileSystem::FileExists(outsideMap)) {
		cout << " [ERROR] Could not find world map solution file: " << outsideMap << std::endl;
		return false;		
	}

	/////////////////////////////////////////////
	// Loading XML WorldMap
	cout << " - Loading worldmap from solution" << std::endl;
	g_game.loadMap();

	/////////////////////////////////////////////
	// Load all NPC's into NpcManager
	cout << " - Loading Npc data" << std::endl;
	if(!m_NpcManager->reload()) {
		cout << " [ERROR] Something went wrong while loading Npc data" << std::endl;
	}

	/*
	/////////////////////////////////////////////
	// Loading sections
	cout << " - Loading encounter sections" << std::endl;
	loadEncounterSections();

	/////////////////////////////////////////////
	// Loading pokecenters
	cout << " - Loading PokeCenter locations" << std::endl;
	loadPokeCenters();

	/////////////////////////////////////////////
	// Loading towns
	cout << " - Loading towns" << std::endl;
	loadTowns();

	/////////////////////////////////////////////
	// Loading quests
	cout << " - Loading quests" << std::endl;
	if(!g_game.loadQuests())
	{
		cout << " [ERROR] Could not load quests." << std::endl;
		return false;
	}
	*/
	cout << std::endl << "* Starting game threads..." << std::endl;

	cout << " - Starting time-out thread" << std::endl;
	myThread = boost::thread(boost::bind(&ServerThread::TimeOutCheck, this));

	// RUN LIKE THE WIND, YOUNG GRASSHOPPER
	cout << " - Starting main game thread" << std::endl;
	g_game.setGameState(GAME_STATE_NORMAL);
	Run();

	return true;
}

void ServerThread::loadEncounterSections()
{
	boost::scoped_ptr<Recordset> sections(new Recordset(database, "SELECT id, encounterRate FROM location_sections"));
	while(sections->moveNext()) {
		g_game.addEncounterSection(sections->getInt("id"), sections->getInt("encounterRate"));
	}
}

void ServerThread::loadPokeCenters()
{
	// Lookup PC in database
	boost::scoped_ptr<Recordset> rst(new Recordset(database, "SELECT pc_id, posX, posY FROM pokecenter"));
	while(rst->moveNext()) {
		int id = rst->getInt("pc_id");
		
		Position pos;
		pos.x = rst->getInt("posX");
		pos.y = rst->getInt("posY");
		pos.z = 1;

		g_game.addPokeCenter(id, pos);
	}
}

void ServerThread::loadTowns()
{
	std::string query = "SELECT id, name, hasPokeCenter, musicID FROM locations";
	boost::scoped_ptr<Recordset> townList(new Recordset(database, query));

	while(townList->moveNext()) 
	{
		g_game.addTown(townList->getInt("id"), townList->getString("name"), townList->getInt("hasPokeCenter"), townList->getInt("musicID"));
	}
}

void ServerThread::StopThread()
{
	_requestStopped = true;
	
	cout << "* Stopping server\n";
	
	// Send disconnect packet to all players
	cout << " - Waiting for all players to disconnect";
	g_game.disconnectAllPlayers();
	
	// Wait untill all clients are disconnected
	do {} while(g_game.getCountPlayers() != 0);
	database->runQuery("UPDATE user SET User_Online='0' WHERE User_Online='1'");
	cout << "\t\t[OK]\n";

	cout << " - Stopping active Npc's";
	m_NpcManager->killThreads();
	cout << "\t\t[OK]\n";

	// Close socket
	cout << " - Closing socket...";
	conn_io_service.post(boost::bind(&ServerThread::closeListenSocket, this));
	Global::setTitle("");
	cout << "\t\t[OK]\n";

	cout << " - Disconnecting from database";
	database->disconnectDB();
	cout << "\t\t[OK]\n";

	cout << " - Interupting socket thread";
	myThread.interrupt();
	cout << "\t\t[OK]\n";

	g_log.WriteLine("--- SERVER STOPPED ---\r\n");
}

void ServerThread::closeListenSocket()
{
	if(m_acceptor){
		if(m_acceptor->is_open()){
			boost::system::error_code error;
			m_acceptor->close(error);
		}
		delete m_acceptor;
		m_acceptor = NULL;
	}
}

void ServerThread::TimeOutCheck()
{
#ifdef __BUGTRAP__
	BT_SetTerminate();
#endif // __BUGTRAP__

	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(10));
		
		boost::recursive_mutex::scoped_lock lock(g_game.player_mutex);
		PlayerList list = g_game.getPlayerList();

		int currentTime = time(0);
		int timePast = 0;
		for(PlayerList::iterator it = list.begin(); it != list.end(); ++it)
		{
			if((*it) && (*it)->client != NULL && !(*it)->isRemoved)
			{
				// Send ping
				if((*it)->sendPing()){
					continue;
				}

				timePast = currentTime - (*it)->client->lastActivity;

				if(timePast >= 600 && timePast <= 610) { // send message to player
					(*it)->sendCancelMessage(RET_YOUAREINACTIVE);
				}
				else if(timePast >= 900) { // Log player out cuz of inactivity
					(*it)->sendDisconnect();
				}
			}
		}
	}
}

void ServerThread::Run()
{
#ifdef __BUGTRAP__
	BT_SetTerminate();
#endif // __BUGTRAP__

	_requestStopped = false;
	md5wrapper md5;

	int serverPort = atoi(g_config.pString("serverPort").c_str());
	std::string myIP = g_config.pString("serverIP");

	tcp::endpoint localEndPoint(tcp::v4(), serverPort);
	m_acceptor = new boost::asio::ip::tcp::acceptor(conn_io_service, localEndPoint);

	conn_io_service.run();

	std::stringstream newTitle;
	newTitle << "- IP: " << myIP << ":" << serverPort;
	Global::setTitle(newTitle.str());
	Global::updatePlayers(0);

	std::vector<string> exclusiveNames;
	exclusiveNames.push_back("mr_dark");
	exclusiveNames.push_back("urmel");
	exclusiveNames.push_back("bakagringo");
	exclusiveNames.push_back("wasabi");
	exclusiveNames.push_back("wasabi-knight");
	exclusiveNames.push_back("kamaran");
	exclusiveNames.push_back("declan_23");
	exclusiveNames.push_back("level5pigey");
	exclusiveNames.push_back("gammal");
	exclusiveNames.push_back("stephenkill");
	exclusiveNames.push_back("raikt");
	exclusiveNames.push_back("spirit42");
	exclusiveNames.push_back("luqman");
	exclusiveNames.push_back("shadowfred");

	cout << std::endl << "* PUServer is now running and accepting new connections..." << std::endl;

	do
	{
		try
		{	
			PUSocket *newSocket = new PUSocket(conn_io_service);
			m_acceptor->accept(newSocket->socket);

			Packet *packet = new Packet();

			boost::system::error_code errorPacket;
			boost::system::error_code error;
			boost::asio::read(newSocket->socket, 
							  boost::asio::buffer(packet->getBuffer(), 2), 
							  boost::asio::transfer_at_least(2), 
							  error);

			if(error && boost::asio::error::would_block != error)
			{
				std::cerr << error.message() << endl;
				
				delete newSocket;
				delete packet;

				continue;
			}

			boost::asio::read(newSocket->socket, 
							  boost::asio::buffer(packet->getDataBuffer(), packet->getHeader()), 
							  boost::asio::transfer_at_least(packet->getHeader()), 
							  error);

			unsigned short header = packet->ReadByte();

			if(!_requestStopped && header == 0x01)
			{
				std::string username, password;
				username = packet->ReadString();
				password = packet->ReadString();
				short versionNumber = packet->ReadInt();

				toLowerCaseString(username);
				std::string::iterator end_pos = std::remove(username.begin(), username.end(), ' ');
				username.erase(end_pos, username.end());

				if(g_game.getGameState() == GAME_STATE_CLOSED || g_game.getGameState() == GAME_STATE_CLOSED) 
				{
					Packet reply;
					reply.AddByte(0x01);
					reply.AddByte(STATUS_SERVERCLOSED);
					reply.Send(newSocket->socket, errorPacket);	
				}
				else if(versionNumber != PUSERV_CLIENT_VERSION)
				{
					Packet reply;
					reply.AddByte(0x01);
					reply.AddByte(STATUS_WRONGVERSION);
					reply.Send(newSocket->socket, errorPacket);	
				}
				else 
				{
					std::vector<string>::iterator derp = std::find(exclusiveNames.begin(), exclusiveNames.end(), username);
					if((username.length() > 1 && username.length() <= 20) && g_game.getPlayerByName(username) == NULL && 
						(derp == exclusiveNames.end() || (derp != exclusiveNames.end() && password == "getreadyforthelaunch")) )
					{
						// Add new player object
						Player *newPlayer = new Player(newSocket, username);

						if(newPlayer && newSocket)
						{
							Global::updatePlayers(g_game.getCountPlayers());

							// Send logged in packet
							Packet reply;
							reply.AddByte(0x01);
							reply.AddByte(STATUS_READY);
							reply.Send(newSocket->socket, errorPacket);

							continue;
						}
					}
					else
					{
						Packet reply;
						reply.AddByte(0x01);
						reply.AddByte(STATUS_WRONGACCOUNT); // User not found
						reply.Send(newSocket->socket, errorPacket);
					}

					/*
					std::transform(username.begin(), username.end(), username.begin(), ::tolower);				
					std::string query = boost::str(boost::format("SELECT User_Password, User_Salt, User_Online, User_GroupID FROM user WHERE User_Name='%s'") % username);
					boost::scoped_ptr<Recordset> rst(new Recordset(database, query));

					if(rst->moveNext())
					{
						//std::stringstream passwd;
						//passwd << password << rst->getString("User_Salt");
						
						if(g_game.getPlayerByName(username))
						{
							Packet reply;
							reply.AddByte(0x01);
							reply.AddByte(STATUS_LOGGEDIN); // User already online
							reply.Send(newSocket->socket, errorPacket);			
						}
						else if(md5.getHashFromString(password) != rst->getString("User_Password"))
						{
							Packet reply;
							reply.AddByte(0x01);
							reply.AddByte(STATUS_WRONGACCOUNT); // User not found
							reply.Send(newSocket->socket, errorPacket);
						}
						else if(rst->getInt("User_GroupID") == 4)
						{
							Packet reply;
							reply.AddByte(0x01);
							reply.AddByte(STATUS_CHARBANNED);
							reply.Send(newSocket->socket, errorPacket);			
						}
						else
						{
							// Add new player object
							Player *newPlayer = new Player(newSocket, username);

							if(newPlayer && newSocket)
							{
								Global::updatePlayers(g_game.getCountPlayers());

								// Send logged in packet
								Packet reply;
								reply.AddByte(0x01);
								reply.AddByte(STATUS_READY);
								reply.Send(newSocket->socket, errorPacket);

								continue;
							}
						}
					}
					else
					{
						Packet reply;
						reply.AddByte(0x01);
						reply.AddByte(STATUS_WRONGACCOUNT); // User not found
						reply.Send(newSocket->socket, errorPacket);
					}*/
				}
			}

			delete newSocket;
			delete packet;
		}
		catch(std::exception& ex)
		{
			std::cerr << "Exception in server thread: " << ex.what() << std::endl;
		}
	} while(!_requestStopped);
	cout << std::endl << "* Server deaded" << std::endl;
}