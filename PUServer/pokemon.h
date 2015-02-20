#ifndef __pokemon_h_
#define __pokemon_h_

#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "database.h"

#include "creature.h" 
#include "attack.h"
#include "statcalculator.h"

enum StatusAilment_t
{
	StatusAilment_BURN = 0,			//2^0 = 1
	StatusAilment_FREEZE,			//2^1 = 2
	StatusAilment_PARALYSIS,		//2^2 = 4
	StatusAilment_POISON,			//2^3 = 8
	StatusAilment_POISON_BAD,		//2^4 = 16
	StatusAilment_SLEEP,			//2^5 = 32
	StatusAilment_ATTRACTION,		//2^6 = 64
	StatusAilment_CONFUSION,		//2^7 = 128
	StatusAilment_CURSE,			//2^8 = 256
	StatusAilment_FLINCH,			//2^9 = 512
	StatusAilment_FORESIGHT,		//2^10 = 1024
	StatusAilment_ODOR_SLEUTH,		//2^11 = 2048
	StatusAilment_MIRACLE_EYE,		//2^12 = 4096
	StatusAilment_LEECH_SEED,		//2^13 = 8192
	StatusAilment_MIND_READER,		//2^14 = 16384
	StatusAilment_LOCK_ON,			//2^15 = 32768
	StatusAilment_NIGHTMARE,		//2^16 = 65536
	StatusAilment_PERISH_SONG,		//2^17 = 131072
	StatusAilment_TAUNT,			//2^18 = 262144
	StatusAilment_TORMENT,			//2^19 = 524288
	StatusAilment_DEFENSE_CURL,		//2^20 = 1048576
	StatusAilment_FOCUS_ENERGY,		//2^21 = 2097152
	StatusAilment_MEAN_LOOK,		//2^22 = 4194304
	StatusAilment_SPIDER_WEB,		//2^23 = 8388608
	StatusAilment_BLOCK,			//2^24 = 16777216
	StatusAilment_MINIMIZE,			//2^25 = 33554432
	StatusAilment_MIST,				//2^26 = 67108864
	StatusAilment_SUBSITUTE,		//2^27 = 134217728
	StatusAilment_REFLECT,			//2^28 = 268435456
	StatusAilment_LIGHT_SCREEN,		//2^29 = 536870912,
	StatusAilment_DESTINY_BOND,		//2^30 = 1073741824,
	StatusAilment_DETECT,			//2^31
	StatusAilment_ENCORE,			//2^32
	StatusAilment_ENDURE,			//2^33
	StatusAilment_MUD_SPORT,		//2^34
};

class TurnMoveInfo 
{
public:
	int foe;
	int turnID;
	int attackID;
	int damage;
	bool hit;
	int playerID;
	bool isPhysical;
	int slotID;
};

struct StatusItem_s
{
	int turnCount;
	int totalDamage;
	int foeID; // Enemy player ID
};

typedef boost::unordered_map<int, StatusItem_s> StatusList; // attackID, StatusItem_s

class Pokemon : public Creature
{
public:
	Pokemon(Database *db, int _uniqueID, bool isPlayerPokemon);
	Pokemon(Database *db, int _pokemonID, int _level);
	~Pokemon();

	Pokemon* getPokemonP() { return this; }
	const Pokemon* getPokemonP() const { return this; }

	PlayerTypes getCreatureType() const { return POKEMON; }

	void savePokemonData(bool saveAttackData=false);

	//std::string getName() { return pokemonName; }
	int getPokemonLvl() { return pokemonLvl; }
	int getPokemonID() { return pokemonID; } // Get Pokemon/Player Unique ID
	int getPokemonDbID() { return pokemonDbID; } // Get Pokemon Database ID
	int getPokemonRealID() { return pokemonRealID; } // Get Pokemon RealID

	int getPokemonHP_Left() { return pokemonHP_Left; }
	int getPokemonHP_Total() { return pokemonHP_Total; }

	long getBaseExp() { return baseExp; }
	long getCurrentExp() { return currentExp; }
	int increaseExp(int ExPoints);
	int getExpPercent();

	int getPokemonSpeed() { return stat_sp; }
	int getGender() { return gender; }

	std::string getType1() { return pokemonType1; }
	std::string getType2() { return pokemonType2; }
	std::string getFlavor() { return flavorText; }
	

	void setPokemonLvl(int _pokemonLvl) { pokemonLvl = _pokemonLvl; }
	void setPokemonHP_Total(int _totalHP) { pokemonHP_Total = _totalHP; }
	void setPokemonHP_Left(int _leftHP) { pokemonHP_Left = _leftHP; }

	void increaseHP(int _addHP);
	void decreaseHP(int _delHP);

	int getHappiness() { return happiness; }
	void setHappiness(int _happiness);

	std::map<int, std::string> getAttackList();
	int getAttackCount() { return attackCount; }
	Attack *getAttack(int _attackID) { return pokemonAttacks[_attackID]; }
	Attack *updateAttack(int _slotID, int _newAttackID);
	int getAttackDamage(int _attackID) { return pokemonAttacks[_attackID]->getAttackPower(); }
	void useAttack(int slotID);

	bool hasUsableAttack();

	void addMoveHistory(int TurnID, TurnMoveInfo moveInfo);

	int getItemID() { return holdingItemID; }
	int getLastAttackID();
	int getLastTurnID();
	TurnMoveInfo getLastTurnInfo();

	int getWeight() { return weight; }
	int getCaptureRate() { return capture_rate; }

	bool setModAc(int amount) { if(mod_ac > -6 && mod_ac < 6) { mod_ac += amount; return true; } else { return false; } }
	bool setModAt(int amount) { if(mod_at > -6 && mod_at < 6) { mod_at += amount; return true; } else { return false; } }
	bool setModDe(int amount) { if(mod_de > -6 && mod_de < 6) { mod_de += amount; return true; } else { return false; } }
	bool setModSa(int amount) { if(mod_sa > -6 && mod_sa < 6) { mod_sa += amount; return true; } else { return false; } }
	bool setModSd(int amount) { if(mod_sd > -6 && mod_sd < 6) { mod_sd += amount; return true; } else { return false; } }
	bool setModSp(int amount) { if(mod_sp > -6 && mod_sp < 6) { mod_sp += amount; return true; } else { return false; } }

	int getModAc() { return mod_ac; }
	int getModAt() { return mod_at; }
	int getModDe() { return mod_de; }
	int getModSa() { return mod_sa; }
	int getModSd() { return mod_sd; }
	int getModSp() { return mod_sp; }

	void resetModDe(int amount) { mod_de = amount; }
	void resetModSd(int amount) { mod_sd = amount; }
	
	void resetStatModifiers()
	{
		mod_ac = 0;
		mod_at = 0;
		mod_de = 0;
		mod_sa = 0;
		mod_sd = 0;
		mod_sp = 0;
	}

	void setStatus(int newStatus) { 
		if(newStatus <= 0 || pokemonStatus <= 0) { 
			pokemonStatus = newStatus; 
			statusCount = 0; 
		} 
	}
	int getStatus() { return pokemonStatus; }

	// Number of rounds a pokemon has this status (SLP, BRN, PRZ, etc)
	void setStatusCount(int count) { statusCount += count; }
	int getStatusCount() { return statusCount; }

	void setFlag(StatusAilment_t value) { StatusAilment |= (1i64 << value); }
	void removeFlag(StatusAilment_t value) { StatusAilment &= ~(1i64 << value); }
	bool hasFlag(StatusAilment_t value) { return (0 != (StatusAilment & (1i64 << value)));}
	long long getFlags() { return StatusAilment; }

	void heal();

	void levelUp(int level);

	int iv_at, iv_de, iv_sa, iv_sd, iv_sp, iv_hp;
	int stat_at, stat_de, stat_sa, stat_sd, stat_sp;
	int mod_ac, mod_at, mod_de, mod_sa, mod_sd, mod_sp;

	std::map<int, TurnMoveInfo> moveHistory;
	StatusList statusTurnList;

	bool doneBattleAttack;
	bool attackLearning;
	int attackLearningID;
	std::string attackLearningName;
	int encoreRounds;

protected:
	void loadPlayerPokemon(int uniqueID);
	void loadNpcPokemon(int pokemonID, int level);

	void loadFlavorText(int pokemonID);
	
private:
	int pokemonID, pokemonDbID, pokemonRealID, pokemonLvl, pokemonHP_Left, pokemonHP_Total, holdingItemID, lastAttackID, capture_rate, attackCount;
	int pokemonStatus, statusCount, happiness, weight, gender;
	std::string pokemonType1, pokemonType2, flavorText;
	bool isWild;
	long baseExp, currentExp;

	int base_at, base_de, base_sa, base_sd, base_sp, base_hp;

	long long StatusAilment;

	Attack *pokemonAttacks[4];
	Database *db;
};

#endif