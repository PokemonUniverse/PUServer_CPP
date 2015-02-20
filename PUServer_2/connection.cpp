#include "connection.h"

#include "tools.h"
#include "logger.h"

#include "netmessage.h"

#include "playerWalkMessage.h"
#include "creatureturnmessage.h"
#include "refreshworldmessage.h"

#include "addcreaturemessage.h"
#include "removecreaturemessage.h"

#include "sendloginmessage.h"
#include "sendplayerdata.h"
#include "sendtilesmessage.h"

#include "creature.h"
#include "player.h"
#include "tile.h"

Connection::Connection( socket_ptr _socket, Player * _player )
{
	m_socket = _socket;
	m_owner = _player;

	m_connectionThread = boost::thread(boost::bind(&Connection::receiveThread, this));
}

void Connection::receiveThread()
{
	for (;;)
	{
		boost::this_thread::interruption_point();

		if(!m_socket->m_socket.is_open()) break;

		packet_ptr packet(new Packet());

		boost::system::error_code error;
		boost::asio::read(m_socket->m_socket, boost::asio::buffer(packet->getBuffer(), 2), boost::asio::transfer_at_least(2), error);

		if(error && boost::asio::error::would_block != error)
		{
			std::stringstream logStr;
			if(m_owner) {
				logStr << "LOGOUT | GUID: " << m_owner->getGUID();
			}
			else {
				logStr << "LOGOUT | GUID: UNKNOWN | IP: UNKNOWN";
			}

			Logger::consoleOutput(COUTPUT_MESSAGE, logStr.str());
		}
		else if (error) {
#ifdef __IS_DEBUG__
			Logger::consoleOutput(COUTPUT_WARNING, boost::str(boost::format("Connection [1] | %1% | %2%: %3%") % m_owner->getGUID() % error.value() % error.message()));
#endif
			continue;
		}

		boost::asio::read(m_socket->m_socket, boost::asio::buffer(packet->getDataBuffer(), packet->getHeader()), boost::asio::transfer_at_least(packet->getHeader()), error);
		//TODO: Fetch real error for 0-byte header here
		if(error) 
		{
			if(boost::asio::error::fault == error) { break;	}
			else if(boost::asio::error::connection_aborted == error) { break; }
			else if(boost::asio::error::eof == error) { break; }
			else {
				Logger::consoleOutput(COUTPUT_WARNING, boost::str(boost::format("Connection [2] | %1% | %2%: %3%") % m_owner->getGUID() % error.value() % error.message()));
			}

			continue;
		}

		parseReceivedPacket(packet);
		m_lastActivity = PUSYS_TIME();
	}

	connectionLost();
}

void Connection::connectionLost()
{
	m_socket->m_socket.close();
	m_owner->removeConnection();
}

void Connection::parseReceivedPacket( packet_ptr packet )
{
	if(!m_owner || packet->getMessageSize() <= 0) return;

	NetMessage* message = NULL;
	unsigned char header = packet->ReadByte();
	switch(header)
	{
	case HEADER_LOGIN:
		sendPlayerData();		
		break;

	case HEADER_WALK:
		message = new CreatureWalkMessage(packet, m_owner);
		break;

	case HEADER_TURN:
		message = new CreatureTurnMessage(packet, m_owner);
		break;

	case HEADER_REFRESHWORLD:
		message = new RefreshWorldMessage(packet, m_owner);
		break;

	default:
		//Logger::consoleOutput(COUTPUT_WARNING, "Received unknown header: " + header);
		return;
	}

	if(message != NULL) {
		message->executeReceivedMessage();
		delete message;
	}
}

void Connection::sendMessage(NetMessage &message )
{
	message.send(m_socket->m_socket);
}

void Connection::sendPlayerData()
{
	SendPlayerData playerData;
	playerData.addGUID(m_owner->getGUID());
	playerData.addName(m_owner->getName());
	playerData.addPosition(m_owner->getPosition());
	playerData.addDirection(m_owner->getDirection());
	playerData.addMoney(m_owner->getMoney());
	playerData.addOutfit(m_owner->getOutfit());
	sendMessage(playerData);

	//TODO: Send items
	//-----------------

	sendMapData(DIR_NULL);

	boost::this_thread::sleep(boost::posix_time::milliseconds(110));

	SendLoginMessage message;
	message.addStatus(STATUS_READY);
	sendMessage(message);
}

void Connection::sendMapData(Direction dir)
{
	int xT = 1, xM = VIEWPORT_X;
	int yT = 1, yM = VIEWPORT_Y;

	if(dir != DIR_NULL)
	{
		switch(dir)
		{
		case DIR_NORTH:
			yM = 1;
			break;
		case DIR_EAST:
			xT = VIEWPORT_X;
			break;
		case DIR_SOUTH:
			yT = VIEWPORT_Y;  
			break;
		case DIR_WEST:
			xM = 1; 
			break;		
		}
	}

	Position position = m_owner->getPosition();
	int playerX = (position.x - ((VIEWPORT_X - 1) / 2));
	int playerY = (position.y - ((VIEWPORT_Y - 1) / 2));
	int playerZ = position.z;

	SendTilesMessage message;
	for(int x = xT; x <= xM; x++)
	{
		for(int y = yT; y <= yM; y++)
		{
			message.addTile((playerX + x), (playerY + y), playerZ);
		}

		if(dir == DIR_NULL) {
			sendMessage(message);
		}
	}

	if(dir != DIR_NULL)
		sendMessage(message);
}

void Connection::sendCreatureMove( Creature* creature, Tile* from, Tile* to )
{
	CreatureWalkMessage message(creature);
	message.addPositions(from->getPosition(), to->getPosition());
	sendMessage(message);
}

void Connection::sendCreatuerTurn( Creature* creature, Direction dir )
{
	CreatureTurnMessage message(creature);
	message.addDirection(dir);
	sendMessage(message);
}

void Connection::sendAddVisibleCreature( Creature* creature )
{
	if(creature->getPlayer() == m_owner) return;

	AddCreatureMessage message;
	message.addCreature(creature);
	message.addOutfit(creature->getOutfit());
	sendMessage(message);
}

void Connection::sendRemoveVisibleCreature( Creature* creature )
{
	if(creature->getPlayer() == m_owner) return;

	RemoveCreatureMessage message(creature);
	sendMessage(message);
}