#ifndef __game_h_
#define __game_h_

#include <iostream>
#include <vector>
#include <map>
#include <boost/thread.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include "definitions.h"
#include "enums.h"
#include "commands.h"
#include "position.h"
#include "cylinder.h"
#include "client.h"
#include "creature.h"
#include "npc.h"
#include "player.h"
#include "map.h"
#include "tile.h"
#include "pokecode.h"
#include "quest.h"
#include "items.h"
#include "towns.h"

typedef std::vector<Player*> PlayerList;
typedef boost::unordered_map<int, int> SectionList;
typedef boost::unordered_map<int, Position> PokeCenters;

class Map;
class Creature;
class Player;
class Npc;
class Items;
class Towns;

extern Commands commands;

enum GameState_t {
	GAME_STATE_STARTUP,
	GAME_STATE_INIT,
	GAME_STATE_NORMAL,
	GAME_STATE_CLOSED,
	GAME_STATE_SHUTDOWN,
	GAME_STATE_CLOSING
};

/**
  * Main Game class.
  * This class is responsible to control everything that happens 
  */
class Game
{
public:
	Game();
	~Game();

	bool saveServer(bool closeServer = false, bool shutDownServer = false);
	bool openServer();
	bool reloadMap();

	//************************************
	// Method:    loadMap
	// FullName:  Game::loadMap
	// Access:    public 
	// Returns:   bool
	// Qualifier: Load the worldmap from solution file
	//************************************
	bool loadMap();

	Map* getMap() { return map; }
	const Map* getMap() const { return map; }

	Tile* getTile(int x, int y);
	Tile* getTile(int x, int y, int z);
	Tile* getTile(const Position pos);

	//************************************
	// Method:    loadQuests
	// FullName:  Game::loadQuests
	// Access:    public 
	// Returns:   bool
	// Qualifier: Loads quests from database
	//************************************
	bool loadQuests();

	//************************************
	// Method:    loadItems
	// FullName:  Game::loadItems
	// Access:    public 
	// Returns:   bool
	// Qualifier: Loads all items from the database
	//************************************
	bool loadItems();

	//************************************
	// Method:    getItems
	// FullName:  Game::getItems
	// Access:    public 
	// Returns:   ItemsList
	// Qualifier: Returns list of all items
	//************************************
	ItemsList getItems() { return items->getItems(); }

	//************************************
	// Method:    getItem
	// FullName:  Game::getItem
	// Access:    public 
	// Returns:   Item*
	// Qualifier: Get specified item. Returns NULL if item doesn't exists
	// Parameter: int _id
	//************************************
	Item* getItem(int _id);

	//************************************
	// Method:    addTown
	// FullName:  Game::addTown
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int _id
	// Parameter: std::string _name
	// Parameter: int _pokeCenter
	// Parameter: int _musicID
	//************************************
	void addTown(int _id, std::string _name, int _pokeCenter, int _musicID);

	//************************************
	// Method:    getTown
	// FullName:  Game::getTown
	// Access:    public 
	// Returns:   Town*
	// Qualifier:
	// Parameter: int _id
	//************************************
	Town* getTown(int _id);

	//************************************
	// Method:    addEncounterSection
	// FullName:  Game::addEncounterSection
	// Access:    public 
	// Returns:   void
	// Qualifier: Adds a encounter section to the map
	// Parameter: int id
	// Parameter: int rate
	//************************************
	void addEncounterSection(int id, int rate);

	//************************************
	// Method:    getCountPlayers
	// FullName:  Game::getCountPlayers
	// Access:    public 
	// Returns:   int
	// Qualifier: Get total online player count
	//************************************
	int getCountPlayers() { return playerList.size(); }

	//************************************
	// Method:    disconnectAllPlayers
	// FullName:  Game::disconnectAllPlayers
	// Access:    public 
	// Returns:   void
	// Qualifier: Disconnects all players from the server
	//************************************
	void disconnectAllPlayers();
	
	/**
	  * Adds a creature to the map
	  * \param creature Pointer to the creature
	  */
	void addCreature(Creature *creature);

	/**
	  * Adds a creature to the map without sending a package
	  * \param creature Pointer to the creature
	  */
	void internalAddCreature(Creature *creature);

	/**
	  * Removes a creature from the map
	  * \param creature is a pointer to the creature
	  */
	void removeCreature(Creature *creature, bool deletePtr = true);

	/**
	  * Returns a creature based on a unique creature identifier
	  * \param creatureID is the unique identifier
	  * \return A Pointer to the creature
	  */
	Creature *getCreatureByID(uint32_t creatureID);

	/**
	  * Returns a creature based on a string name identifier
	  * \param s is the name identifier
	  * \return A Pointer to the creature
	  */
	Creature* getCreatureByName(const std::string& s);
	
	/**
	  * Returns a player based on a unique player identifier
	  * \param playerID is the unique identifier
	  * \return A Pointer to the player
	  */
	Player *getPlayerByID(uint32_t playerID);
	Player *getPlayerByUID(uint32_t playerUID);

	/**
	  * Returns a player based on a string name identifier
	  * \param s is the name identifier
	  * \return A Pointer to the player
	  */
	Player *getPlayerByName(const std::string &s);

	/**
	  * nigger nigger nigger nigger nigger nigger
	  * \param nigger is the nigger identifier
	  * \return A nigger to the nigger
	  */

	void getCreaturesByPos(int x, int y, int z, CreatureList &list);

	/**
	  * Teleports an object to another position
	  * \param thing is the object to teleport
	  * \param newPos is the new position
	  * \param flags optional flags to modify default behavior
	  * \return true if the teleportation was successful
	  */
	ReturnValue internalTeleport(Thing* thing, const Position& newPos, uint32_t flags = 0);

	/**
	  * Turn a creature to a different direction.
	  * \param creature Creature to change the direction
	  * \param dir Direction to turn to
	  */
	bool internalCreatureTurn(Creature* creature, Direction dir);

	/**
	  * Moves a creature to a different direction.
	  * \param creature Creature to change the direction
	  * \param dir Direction to move to
	  */
	ReturnValue internalCreatureMove(Creature* creature, Direction dir);

	//************************************
	// Method:    internalMoveCreaturePokeCenter
	// FullName:  Game::internalMoveCreaturePokeCenter
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: Creature * creature
	//************************************
	bool internalMoveCreaturePokeCenter(Creature* creature);

	/**
	  * Creature wants to say something.
	  * \param creature Creature pointer
	  * \param type Type of message
	  * \param text The text to say
	  */
	bool internalCreatureSay(Creature* creature, SpeakClasses type, const std::string& text, int channelId = 0);
	
	bool internalBroadcastMessage(Player* player, const std::string& text);


	void addCommandTag(std::string tag);
	void resetCommandTag();
	bool playerSayCommand(Player* player, SpeakClasses type, const std::string& text);

	bool globalCreatureSay(Creature* creature, const std::string& text, int channelId = 1);
	bool playerPrivateMessage(Creature* creature, const std::string& text, int channelId);
	bool playerBattleChat(Creature *creature, const std::string& text);

	// Implementation of player invoked actions
	bool playerMove(Player *player, Direction direction, bool sendMap = true);
	bool playerTurn(Player *player, Direction direction);
	bool playerSay(Player *player, uint16_t channelId, SpeakClasses type, const std::string& receiver, const std::string& text);
	bool playerYell(Player *player, const std::string& text);
	bool playerWhisper(Player *player, const std::string& text);
	void playerChangeOutfit(Player *player, CreatureOutfit outfit);

	void addPokeCenter(int id, Position coords);
	Position getPokeCenter(int _id);
	
	PlayerList getPlayerList() const { return playerList; }
	bool checkWilEncounter(Creature *player, int sectionID);
	CreatureList getCreatureList() const { return creatureList; }

	GameState_t getGameState() { return gameState; }
	void setGameState(GameState_t _state) { gameState = _state; }
	
	//boost::mutex player_mutex;
	boost::recursive_mutex player_mutex;
	boost::recursive_mutex creature_mutex;
	boost::mutex creature_mutex2;
protected:
	GameState_t gameState;

	Map *map;
	Quests *quests;
	Items *items;
	Towns *towns;

	CreatureList creatureList;
	PlayerList playerList;
	SectionList encounterSections;
	PokeCenters pokeCenterList;

	std::vector<std::string> commandTags;
};

#endif