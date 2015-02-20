#ifndef INC_HAILEFFECT_H_
#define INC_HAILEFFECT_H_

#include "definitions.h"
#include "weathereffect.h"
#include "statusmove.h"

class Pokemon;
class BattleField;
class MoveListEntry;
class BattleMechanics;

/**
 * 1. Hurts all pokemon with 1/16 HP if they are not Ice types.
 * 2. Cuts the power of Solarbeam to 60.
 * 3. Makes Weather Ball a power 100 Ice-type move.
 * 4. Makes Moonlight and Morning Sun restore 1/4 of the user's max HP.
 * 5. Increases the evasion of a Pokemon with Snow Cloak by 20%
 */
class HailEffect : public WeatherEffect
{
public:
	HailEffect() : WeatherEffect(5) {};
	HailEffect(int turns) : WeatherEffect(turns) {};

	std::string getName() { return "Hail"; };
	std::string getDescription() { return ""; };

	bool immobilises(Pokemon* p) { return false; };

	bool applyToField(BattleField* field);
	void unapplyToField(BattleField* field);

	/**
     * Does 1/16 of a Pokemon's health worth of damage each turn if the Pokemon is not
     * Ice type.
     */
	bool tickPokemon(Pokemon* p);

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);
	
	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

protected:
	void tickWeather(BattleField* field);
};

class HailEffectBlizzard : public StatusMove
{
public:
	HailEffectBlizzard(statusEffectList_t statuses, boolArray_t attacker, doubleArray_t chances)
		: StatusMove(PokemonType::T_ICE, 120, 0.7, 5, statuses, attacker, chances) {};
	~HailEffectBlizzard() {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};

#endif