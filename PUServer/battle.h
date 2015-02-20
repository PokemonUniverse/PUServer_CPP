#ifndef __battle_h_
#define __battle_h_

#include <iostream>
#include <map>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include "creature.h"
#include "database.h"
#include "pokemon.h"
#include "statcalculator.h"
#include "gametext.h"

class BattlePlayer;
class Pokemon;

struct StatusItem_s;

using namespace std;

enum Battle_t
{
	ONE_VS_ONE = 1,
	TWO_VS_ONE = 2,
	ONE_VS_TWO = 3,
	TWO_VS_TWO = 4
};

enum BattleState_t
{
	STATE_WAITING = 0,
	STATE_RUNNING = 1,
	STATE_STOPPING = 2,
	STATE_ROUNDFINISH = 3,
	STATE_FINISHED = 4
};

struct WildPokemonEncounter {
    int pokemonID;
    int minLevel;
    int maxLevel;

	WildPokemonEncounter() {}
	WildPokemonEncounter( int id, int min, int max ) : pokemonID(id), minLevel(min), maxLevel(max) {}
};

typedef std::map<int, WildPokemonEncounter> EncounterMap;

class Battle
{
public:
	Battle(Battle_t type);
	~Battle();

	/**
	  * Add a creature (Player/Npc) to the battle
	  */
	bool addPlayer(Creature *creature, int team);
	/**
	  * Add a pokemon to the battle from pokemonID and level
	  */
	bool addPlayer(int pokemonID, int level, int team);
	/**
	  * Add a pokemon to the battle, generated from section ID
	  */
	int addPlayer(int sectionID, int team);
	/**
	  * Removes a player from the battle
	  */
	void removePlayer(BattlePlayer *player, bool punishPlayer = false);
	/**
	  * Get battle player by battle player id
	  */
	BattlePlayer* getPlayer(int playerID);
	/**
	  * Get number of active players per team
	  */
	int getPlayersTeam(int team);
	/**
	  * Get total players
	  */
	int getPlayersTotal();
	/**
	  * Tries to start the battle, if there are enough players
	  */
	bool startBattle();
	/**
	  * Stops the battle
	  */
	void endBattle();

	/**
	  * Checks if there are enough players left to continue
	  */
	bool checkBattleContinue();

	BattlePlayer *getBattlePlayerByPlayer(Player *player);

	std::vector<Creature*> getPlayerList();

	void validateRun(Player* player);
	void validatePokemonChange(Player* player, int pokemonSlot);
	void validateAttack(Player* player, int attackSlot);
	void validateAnswer(Player* player, int parameter);
	void validateItem(Player* player, int _category, int _slot);

protected:
	/**
	  * Checks if there is place in the team
	  */
	bool checkPlaceInTeam(int team);
	/**
	  * Sets up when each player does it's move
	  */
	void setupBattleOrder();
	/**
	  * Main battle loop
	  */
	void run();

	void preBattleData();

	void autoMoveChooser(BattlePlayer *player);

	void moveRun(BattlePlayer *attacker);
	void moveAttack(BattlePlayer *player);
	void moveChangePokemon(BattlePlayer *player);
	void moveUseItem(BattlePlayer *player);
	bool moveUsePokeball(BattlePlayer *player);

	void checkFaintedPokemon(BattlePlayer *attacker, BattlePlayer *defender);
	void pokemonFainted(BattlePlayer *winrar, BattlePlayer *loser);
	
	void checkPokemonAttackLearn(BattlePlayer *player);
	void assignPokemonAttack(BattlePlayer *player);

	bool roundCleanUp(); // Returns true if the battle is over
	void deleteBattlePlayer(BattlePlayer *_player);
	bool preMoveStatusCheck(BattlePlayer *player);

	void doAttackTurnDamage(int attackID, StatusItem_s statusItem, BattlePlayer *attacker);
	void addAttackTurnDamage(int attackID, BattlePlayer *defender, Pokemon* defendPokemon, int enemyID, int attackDamage);
	bool checkStatusFlags(BattlePlayer *player);
private:
	Database *db;
	boost::thread battleThread;

	typedef boost::unordered_map<int, BattlePlayer*> BattleList;
	BattleList battlePlayerList; // Active players in the battle

	Battle_t battleType;
	BattleState_t battleState;
	int minPlayers, maxPlayers, totalPlayers, playerCounter;
	int turnMoveID;
	bool freePokemonChange;
	bool isWildBattle;
};
#endif