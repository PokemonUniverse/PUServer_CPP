#ifndef INC_CONNECTION_H_
#define INC_CONNECTION_H_

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "definitions.h"

#include "netmessage.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

enum LoginStatus
{
	STATUS_IDLE = 0,
	STATUS_WRONGACCOUNT = 1,
	STATUS_SERVERERROR = 2,
	STATUS_DATABASEERROR = 3,
	STATUS_LOGGEDIN = 4,
	STATUS_READY = 5,
	STATUS_CHARBANNED = 6,
	STATUS_SERVERCLOSING = 7,
	STATUS_SERVERCLOSED = 8,
	STATUS_WRONGVERSION = 9,
	STATUS_FAILPROFILELOAD = 10,
};

class Creature;
class Player;
class Tile;

struct SocketPtr;
typedef boost::shared_ptr<SocketPtr> socket_ptr;

struct SocketPtr
{
	SocketPtr(boost::asio::io_service &io_service) : m_socket(io_service) {};
	~SocketPtr() {};

	boost::asio::ip::tcp::socket m_socket;
};

class Connection
{
	socket_ptr m_socket;
	Player* m_owner;

	int64_t m_lastActivity;
	int64_t m_lastMessage;

	boost::thread m_connectionThread;
	void receiveThread();

	void connectionLost();

	void parseReceivedPacket(packet_ptr packet);
	void sendPlayerData();

public:
	Connection(socket_ptr _socket, Player* _player);
	~Connection() {};

	void sendMessage(NetMessage &message);

	int64_t getLastActivity() { return m_lastActivity; };

	// Public connection methods
	void sendMapData(Direction dir);
	void sendCreatureMove(Creature* creature, Tile* from, Tile* to);
	void sendCreatuerTurn(Creature* creature, Direction dir);
	void sendAddVisibleCreature(Creature* creature);
	void sendRemoveVisibleCreature(Creature* creature);
};
#endif