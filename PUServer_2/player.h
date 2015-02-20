#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "definitions.h"
#include "creature.h"
#include "connection.h"

class Player : public Creature
{
public:
	Player(const std::string& _name);
	~Player() {};

	virtual Player* getPlayer() { return this; };
	virtual const Player* getPlayer() const { return this; };

	void setID(uint32_t idx) { m_idplayer = idx; };
	uint32_t getID() { return m_idplayer; };

	virtual const std::string& getNameDescription() const { return m_name; };
	virtual uint32_t idRange() { return 0x10000000; };

	//************************************
	// Method:    assignConnection
	// FullName:  Player::assignConnection
	// Access:    public 
	// Returns:   void
	// Qualifier: Assigns a new socket to the player object, will only work if the current connection is NULL
	// Parameter: Connection * _connection
	//************************************
	void assignConnection(Connection* _connection);

	//************************************
	// Method:    removeConnection
	// FullName:  Player::removeConnection
	// Access:    public 
	// Returns:   void
	// Qualifier: Removes the current connection. Happens when socket endpoint is lost
	//************************************
	void removeConnection();

	//************************************
	// Method:    hasConnection
	// FullName:  Player::hasConnection
	// Access:    public 
	// Returns:   bool
	// Qualifier: Returns true if the player object has a socket connection
	//************************************
	bool hasConnection() { return (m_connection != NULL); };

	//************************************
	// Method:    sendMessage
	// FullName:  Player::sendMessage
	// Access:    public 
	// Returns:   void
	// Qualifier: Send NetMessage to the player
	// Parameter: const NetMessage & message
	//************************************
	void sendMessage(NetMessage &message) { m_connection->sendMessage(message); };

	int64_t getConnectionLostTime() { return m_connectionLostTime; };
	int64_t getLastActivityTime();

	void setTown(Town* town) { m_lastTown = town; };
	Town* getTown() { return m_lastTown; };

	void setPokeCenter(Position pokecenter) { m_lastPokeCenter = pokecenter; };
	Position getPokeCenter() { return m_lastPokeCenter; };

	void setMoney(int32_t money) { m_money = ( ((m_money + money) < 0) ? 0 : (m_money += money)); };
	uint32_t getMoney() { return m_money; };

	void removeVisibleCreature(Creature *creature);
	void addVisibleCreature(Creature *creature);

	//////////////////////////////////////////////////////////////////////////
	// Inherited functions
	//////////////////////////////////////////////////////////////////////////
	void onCreatureAppear(Creature* creature, bool isLogin);
	void onCreatureDisappear(Creature* creature, bool isLogout);
	void onCreatureMove(Creature* creature, Tile* from, Tile* to, bool teleport = false);
	void onCreatureTurn(Creature* creature, Direction dir);

	//////////////////////////////////////////////////////////////////////////
	// Connection functions
	//////////////////////////////////////////////////////////////////////////
	void sendMapData(Direction dir) { m_connection->sendMapData(dir); };
	void sendCreatureMove(Creature* creature, Tile* fromTile, Tile* toTile) { m_connection->sendCreatureMove(creature, fromTile, toTile); };
	void sendCreatureTurn(Creature* creature, Direction dir) { m_connection->sendCreatuerTurn(creature, dir); };

private:
	Connection* m_connection;
	int64_t m_connectionLostTime;

	bool m_isLoaded;

	uint32_t m_idplayer; // database id
	uint32_t m_money;

	Town* m_lastTown;
	Position m_lastPokeCenter;
};

#endif