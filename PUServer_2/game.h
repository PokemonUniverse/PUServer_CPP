#ifndef __GAME_H_
#define __GAME_H_

#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "position.h"

class Creature;
class Player;
class IOGame;
class Map;
class Tile;

enum GameState_t {
	GAME_STATE_STARTUP,
	GAME_STATE_INIT,
	GAME_STATE_NORMAL,
	GAME_STATE_CLOSED,
	GAME_STATE_SHUTDOWN,
	GAME_STATE_CLOSING
};

typedef boost::unordered_map<uint32_t, Creature*> creatureList_t;
typedef boost::unordered_map<uint32_t, Player*> playerList_t;

class Game
{
public:
	Game();
	~Game();

	bool loadGame();
	bool stopGame();
	bool reloadGame();

	GameState_t getGameState() { return m_gamestate; };

	//************************************
	// Method:    getPlayerByID
	// FullName:  Game::getPlayerByID
	// Access:    public 
	// Returns:   Player*
	// Qualifier: Get player object by user id, returns NULL if not found
	// Parameter: uint32_t idx
	//************************************
	Player* getPlayerByID(uint32_t idx);

	//************************************
	// Method:    getCreatureByGUID
	// FullName:  Game::getCreatureByGUID
	// Access:    public 
	// Returns:   Creature*
	// Qualifier: Get creature object by GUID (Game Unique ID), returns NULL if not found
	// Parameter: uint32_t idx
	//************************************
	Creature* getCreatureByGUID(uint32_t idx);

	//************************************
	// Method:    getTile
	// FullName:  Game::getTile
	// Access:    public 
	// Returns:   Tile*
	// Qualifier: Get tile with position from Map, NULL if non-existent
	// Parameter: const Position position
	//************************************
	Tile* getTile(const Position position);

	//************************************
	// Method:    getTile
	// FullName:  Game::getTile
	// Access:    public 
	// Returns:   Tile*
	// Qualifier: Get tile from world coordinates, NULL if non-existent
	// Parameter: int32_t x
	// Parameter: int32_t y
	// Parameter: int32_t z
	//************************************
	Tile* getTile(int32_t x, int32_t y, int32_t z);

	//************************************
	// Method:    addCreature
	// FullName:  Game::addCreature
	// Access:    public 
	// Returns:   void
	// Qualifier: Add new creature to the game and make it knows to surrounding creature
	// Parameter: Creature * creature
	//************************************
	void addCreature(Creature *creature);

	//************************************
	// Method:    removeCreature
	// FullName:  Game::removeCreature
	// Access:    public 
	// Returns:   void
	// Qualifier: Remove creature from the game, delete from surroundings
	// Parameter: Creature * creature
	// Parameter: bool needLock - True if this method needs to lock the write mutex
	//************************************
	void removeCreature(Creature *creature, bool needLock = true);

	//************************************
	// Method:    onPlayerMove
	// FullName:  Game::onPlayerMove
	// Access:    public 
	// Returns:   bool
	// Qualifier: Method activated when a player moves
	// Parameter: const Player * player
	// Parameter: Direction direction - direction the player moves to
	// Parameter: bool sendMapUpdate - true if the client need new tiles
	//************************************
	bool onPlayerMove(Player* player, Direction direction, bool sendMapUpdate = true);

	//************************************
	// Method:    onPlayerTurn
	// FullName:  Game::onPlayerTurn
	// Access:    public 
	// Returns:   bool
	// Qualifier: Method which is activated when a player turns
	// Parameter: Player * player
	// Parameter: Direction direction - new direction the player is looking at
	//************************************
	bool onPlayerTurn(Player* player, Direction direction);

	//************************************
	// Method:    internalCreatureMove
	// FullName:  Game::internalCreatureMove
	// Access:    public 
	// Returns:   ReturnValue
	// Qualifier: Moves a creature internally
	// Parameter: Creature * creature
	// Parameter: Direction dir
	//************************************
	ReturnValue internalCreatureMove(Creature* creature, Direction dir);

	//************************************
	// Method:    internalCreatureTeleport
	// FullName:  Game::internalCreatureTeleport
	// Access:    public 
	// Returns:   ReturnValue
	// Qualifier: Internally teleport a creature to new position
	// Parameter: Creature * creature
	// Parameter: Tile * from
	// Parameter: Tile * to
	//************************************
	ReturnValue internalCreatureTeleport(Creature* creature, Tile* from, Tile* to, bool internalOnly = false);

	//************************************
	// Method:    internalCreatureTurn
	// FullName:  Game::internalCreatureTurn
	// Access:    public 
	// Returns:   ReturnValue
	// Qualifier: Internal method for turning creatures
	// Parameter: Creature * creature
	// Parameter: Direction dir
	//************************************
	ReturnValue internalCreatureTurn(Creature* creature, Direction dir);

private:
	friend class IOGame;
	friend class Server;

	boost::shared_mutex m_creatureMutex;

	GameState_t m_gamestate;

	creatureList_t m_creatureList;
	playerList_t m_playerList;

	Map *m_map;
};

#endif