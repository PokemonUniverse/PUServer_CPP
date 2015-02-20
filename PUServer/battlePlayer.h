#ifndef __battlePlayer_h_
#define __battlePlayer_h_

#include <iostream>

#include "enums.h"
#include "database.h"
#include "creature.h"
#include "packet.h"
#include "pokemon.h"

class Player;

enum BPStatus_t
{
	BPStatus_TURNLOCKED = 0,	// User can not attack or switch next turn
	BPStatus_SWITCHLOCK,		// Target can not switch pokemon
	BPStatus_HEALINGWISH,		// The next Pokémon switched in heals all HP and status
};

// Generic class to access normal players and NPC's from the battle class
class BattlePlayer
{
public:
	BattlePlayer(Creature *creature);
	BattlePlayer(Database *db, int pokemonId, int level);
	~BattlePlayer();

	PlayerTypes getType() { return playerType; }
	PlayerTypes getCreatureType() { return realPlayerType; }
	std::string getName();
	Creature* getCreature();

	void sendDataPacket(Packet packet);

	int getBattleID() { return battleID; }
	void setBattleID(int id) { battleID = id; }

	int getTeam() { return team; }
	void setTeam(int _team) { team = _team; }

	Pokemon* getPokemon(int slot);
	Pokemon* getPokemonActive();
	int getPokemonActiveSlot();
	int getPokemonTotal();
	int getPokemonTotalActive();
	PokemonList getPokemonList();

	void setPokemon(int slot);

	bool checkPokemonNext();

	BattlePlayerStatus_t getPlayerStatus();
	void setPlayerStatus(BattlePlayerStatus_t newStatus);

	MoveType_t getMoveOption();
	void setMoveOption(MoveType_t option);

	long getMoveParameter();
	void setMoveParameter(long param);

	void setMoveValidated(int param);

	void setAttackCharge(int amount);
	int getAttackCharge();
	
	void resetBattleInfo();
	void setActivePokemon();
	bool isRemoved;

	void setFlag(BPStatus_t value) { battleFlags |= (1 << value); }
	void removeFlag(BPStatus_t value) { battleFlags &= ~(1 << value); }
	bool hasFlag(BPStatus_t value) { return (0 != (battleFlags & (1 << value)));}
	int getFlags() { return battleFlags; }

	bool isFainted;

private:
	Creature *myPlayer;
	PlayerTypes playerType;
	PlayerTypes realPlayerType;

	int team, battleID;

	int battleFlags;

	// Dummy variables for wild pokemon
	BattlePlayerStatus_t playerStatus;
	Pokemon* myPokemon;

	MoveType_t moveOption;
	int moveValidated;
	long moveParameter;
	int attackCharge;
};

#endif