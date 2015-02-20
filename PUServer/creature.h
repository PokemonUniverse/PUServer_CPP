#ifndef __creature_h_
#define __creature_h_

#include "global.h"
#include "position.h"
#include "tile.h"
#include "depot.h"
#include "items.h"

#include <vector>
#include <algorithm>
#include <boost/unordered_map.hpp>

class Thing;
class Player;
class Npc;
class Depot;
class Item;
class Tile;
class Battle;
class Pokemon;

using namespace std;

typedef std::vector<unsigned long long> GUIDList;
typedef std::vector<Creature*> CreatureList;
typedef boost::unordered_map<int, Pokemon*> PokemonList;

#define EVENT_CREATURE_THINK_INTERVAL 1000

struct CreatureOutfit
{
	int head_style;
	int head_colour;
	int nek_style;
	int nek_colour;
	int upper_style;
	int upper_colour;
	int lower_style;
	int lower_colour;
	int feet_style;
	int feet_colour;
};

class Creature : public virtual Thing
{
protected:
	Creature();
public:
	virtual ~Creature();

	virtual Creature* getCreature() { return this; }
	virtual const Creature* getCreature() const { return this; }
	virtual Player* getPlayer() { return NULL; }
	virtual const Player* getPlayer() const { return NULL; }
	virtual Npc* getNpc() { return NULL; }
	virtual const Npc* getNpc() const { return NULL; }
	virtual Pokemon* getPokemonP() { return NULL; }
	virtual const Pokemon* getPokemonP() const { return NULL; }

	virtual PlayerTypes getCreatureType() const { return CREATURE; }

	virtual void onCreatureMove(const Creature* creature, Cylinder* fromCylinder, Cylinder* toCylinder);
	virtual void onCreatureSay(const Creature* creature, SpeakClasses type, std::string text) { }

	virtual void removeVisibleCreature(const Creature *creature);
	virtual void addVisibleCreature(const Creature *creature);

	bool canSee(const Creature* creature);
	bool canSee(const Position& pos);
	bool knowsCreature(const Creature *creature);

	void setName(std::string newName) { name = newName; }
	std::string getName() const { return name; }

	playersex_t getSex() const { return sex; }
	bool getPremium() const { return isPremium; }

	int getID() { return m_id; }
	uint32_t getGUID() const { return GUID; }

	void setOutfit(CreatureOutfit _outfit) { outfit = _outfit; }
	CreatureOutfit getOutfit() { return outfit; }

	// Position
	void setPosition(int x, int y) {
		creaturePosition.x = x;
		creaturePosition.y = y;
	}
	void setPosition(int x, int y, int z) {
		creaturePosition.x = x;
		creaturePosition.y = y;
		creaturePosition.z = z;
	}
	void setPosition(Position pos) {
		creaturePosition = pos;
	}

	int getX() { return creaturePosition.x; }
	int getY() { return creaturePosition.y; }
	int getZ() { return creaturePosition.z; }

	const Position getPosition() const { return creaturePosition; }

	Direction getDirection() { return creatureDirection; }
	void setDirection(Direction dir) { creatureDirection = dir; }
	void setInvisible(bool invisible) { isInvisible = invisible; }
	bool getInvisible() { return isInvisible; }

	void setAdminRepel(bool repel) { isAdminRepel = repel; }
	bool getAdminRepel() { return isAdminRepel; }

	int getMovement() { return movement; }
	void setMovement(int _move) { movement = _move; }
	
	int getSpeed() { return speed; }
	int64_t getTimeSinceLastMove() const;

	CreatureList getVisibleCreatures() const { return visibleCreatures; }
	PokemonList getPokemonParty() { return pokemonParty; }
	void changePokemonSlot(int oldSlot, int newSlot);

	int getLevel() { return level; }
	void setLevel(int _level) { level = _level; }
	int getGuildId() { return 0; }

	// Battle stuff
	bool getBattle() { return isInBattle; }
	Battle* getBattlePointer() { return battle; }
	BattlePlayerStatus_t getPlayerStatus() { return playerStatus; }
	//Pokemon *getPokemon(int slotID) { return pokemonParty[slotID]; }
	Pokemon *getPokemon(int slotID) 
	{ 
		PokemonList::iterator it = pokemonParty.find(slotID);
		if(it == pokemonParty.end()) {
			return NULL;
		}

		return (Pokemon*)it->second;
	}
	Pokemon *getActivePokemon() 
	{ 
		if(!getPokemon(activePokemonSlot)) {
			setActivePokemon();
		}
		return getPokemon(activePokemonSlot); 
	}
	int getActivePokemonSlot() { return activePokemonSlot; }
	int getPokemonCount() { return pokemonParty.size(); }
	int getPokemonCountActive();
	MoveType_t getMoveOption() { return moveOption; }
	long getMoveParameter() { return moveParameter; }

	void setBattle(bool isBattle) { isInBattle = isBattle; }
	void setBattlePointer(Battle *_battle) { battle = _battle; }
	void setPlayerStatus(BattlePlayerStatus_t newStatus) { playerStatus = newStatus; }
	void setPokemon(int slotID, Pokemon *pokemonInfo) { pokemonParty[slotID] = pokemonInfo; }
	void setActivePokemon();
	void setActivePokemon(int slotID);
	void setMoveOption(MoveType_t option) { moveOption = option; }
	void setMoveParameter(long param) { moveParameter = param; }
	void setMoveValidated(int param) { moveValidated = param; }

	void setBattleAttackCharge(int amount) { attackCharging = amount; }
	int getBattleAttackCharge() { return attackCharging; }

	virtual void pokemonLearnAttackSequence() {};
	virtual void savePokemon(int slotID = -1) {};

	void removeFromBattle();
	void resetBattleInfo();
	bool checkNextPokemon();

	bool isRemoved;

	// Items
	Depot* getBackPack() { return backPack; }
	Depot* getItemDepot() { return depotItems; }

	void addItem(int _itemID, int _count);

protected:
	Battle* battle;

	CreatureList visibleCreatures;
	PokemonList pokemonParty;

	Position creaturePosition;
	Direction creatureDirection;

	playersex_t sex;
	CreatureOutfit outfit;

	std::string name;

	int m_id;
	int GUID;
	int movement;
	int speed;
	int lastStep;
	int pokemonCount;
	int activePokemonSlot;
	int level;

	bool isPremium;
	bool isInvisible;
	bool isAdminRepel;

	// Battle stuff
	BattlePlayerStatus_t playerStatus;
	
	bool isInBattle;
	bool freePokemonChange;

	int battlePosition;
	MoveType_t moveOption;
	long moveParameter;
	int moveValidated;
	int moveLearnID;
	int attackCharging;

	Depot *backPack;
	Depot *depotItems;

private:
	static uint64_t count;
	static GUIDList UniqueIdList;
};

#endif