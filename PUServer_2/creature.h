#ifndef __CREATURE_H_
#define __CREATURE_H_

#include "definitions.h"
#include "templates.h"

#include "thing.h"

#include "position.h"
#include "tile.h"
#include "enums.h"
#include "condition.h"
#include "outfit.h"

#include <vector>
#include <list>
#include <boost/unordered_map.hpp>

class Map;
class Thing;
class Container;
class Player;
class Pokemon;
class PokemonSpecies;
class PokemonParty;
class BattleField;
class BattleMechanics;
class Npc;
class Item;
class Tile;

typedef std::vector<Creature*> CreatureList;

enum CreatureType { 
	PLAYER, 
	NPC, 
	POKEMON 
};

class Creature : public AutoID, virtual public Thing
{
protected:
	Creature();

public:
	virtual ~Creature();

	virtual Creature* getCreature() { return this; };
	virtual const Creature* getCreature()const { return this; };
	virtual Player* getPlayer() { return NULL; };
	virtual const Player* getPlayer() const { return NULL; };
	virtual Npc* getNpc() { return NULL; };
	virtual const Npc* getNpc() const { return NULL; };
	virtual Pokemon* getPokemon() { return NULL; };
	virtual const Pokemon* getPokemon() const { return NULL; };
	virtual PokemonSpecies* getPokemonSpecies() { return NULL; };
	virtual const PokemonSpecies* getPokemonSpecies() const { return NULL; };

	virtual const std::string& getNameDescription() const = 0;
	virtual std::string getDescription(int32_t lookDistance) const { return ""; };

	virtual uint32_t idRange() = 0;

	bool canSee(Creature* creature);
	bool canSee(Position& pos);

	virtual void removeVisibleCreature(Creature *creature);
	virtual void addVisibleCreature(Creature *creature);
	virtual bool knowsVisibleCreature(Creature *creature);
	CreatureList getVisibleCreatures() { return m_visibleCreatures; };

	void setWalkSpeed(uint32_t _speed) { m_walkSpeed = _speed; };
	uint32_t getWalkSpeed() { return m_walkSpeed; };
	uint64_t getTimeSinceLastMove() const;
	bool canMove();
	
	void setMovement(creaturemovement_t movement) { m_movement = movement; };
	const creaturemovement_t getMovement() const { return m_movement; };

	// Battle
	virtual void attachToField(BattleField* field, uint8_t team, uint8_t pos);
	uint8_t getBattleTeamIdx() { return m_battleTeam; };
	uint8_t getBattleTeamPosition() { return m_battleTeamPos; };
	bool isInBattle() const { return (m_field != NULL); };
	BattleField* getBattleField() { return m_field; };

	std::string getName() { return m_name; };
	void setName(std::string name) { m_name = name; };

	void setGUID() { this->m_id = auto_id | this->idRange(); }
	uint32_t getGUID() const { return m_id; }

	void setRemoved() { isInternalRemoved = true; }

	Direction getDirection() { return m_direction; }
	void setDirection(Direction dir) { m_direction = dir; }

	int getGender() { return m_gender; };

	const Position& getMasterPos() const { return masterPos;}
	void setMasterPos(const Position& pos) { masterPos = pos; }

	virtual int getThrowRange() const { return 1; };
	virtual bool isRemoved() const { return isInternalRemoved; };
	virtual bool canSeeInvisibility() const { return false; }
	//bool isInvisible() const { return hasCondition(CONDITION_INVISIBLE); }

	void setMaster(Creature* creature) { master = creature; }
	Creature* getMaster() { return master;}
	bool isSummon() { return master != NULL; }
	bool isPlayerSummon() { return master != NULL && master->getPlayer(); }

	PokemonParty* getPokemonParty() { return m_pokeParty; };

	void setOutfit(Outfit outfit) { m_outfit->update(outfit); };
	Outfit* getOutfit() { return m_outfit; };

	virtual void setTile(Tile* tile) { m_tile = tile; }
	virtual Tile* getTile() { return m_tile; }
	virtual Position getPosition() { return ((!m_tile) ? Position::PosNULL() : m_tile->getPosition()); }

	virtual void onCreatureAppear(Creature* creature, bool isLogin) {};
	virtual void onCreatureDisappear(Creature* creature, bool isLogout) {};
	virtual void onCreatureMove(Creature* creature, Tile* from, Tile* to, bool teleport = false);

	virtual void onCreatureTurn(Creature* creature, Direction dir) { };
	virtual void onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text) { };
	virtual void onCreatureChangeOutfit(Creature* creature, Outfit_t& outfit) { };
	virtual void onCreatureChangeVisible(Creature* creature, bool visible) {};
	
protected:
	CreatureType m_creatureType;
	Outfit* m_outfit;

	int m_gender;

	Tile* m_tile;
	uint32_t m_id;
	bool isInternalRemoved;

	Position masterPos;
	int32_t masterRadius;
	Direction m_direction;

	uint64_t m_lastStep;
	uint32_t m_walkSpeed;
	creaturemovement_t m_movement;
	
	//Battle
	BattleField* m_field;
	BattleMechanics* m_mech;
	uint8_t m_battleTeam;
	uint8_t m_battleTeamPos;

	//summon variables
	Creature* master;
	std::list<Creature*> summons;
	PokemonParty* m_pokeParty;

	CreatureList m_visibleCreatures;

	std::string m_name;

	//friend class Commands;
	//friend class LuaScriptInterface;
};

#endif