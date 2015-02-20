#ifndef INC_SANDSTORMEFFECT_H_
#define INC_SANDSTORMEFFECT_H_

#include "definitions.h"
#include "weathereffect.h"

class Pokemon;
class BattleField;
class MoveListEntry;

/**
 * 1. Hurts all pokemon with 1/16 HP unless they are Rock/Ground/Steel types or have the ability Sand Veil.
 * 2. Cuts the power of Solarbeam to 60.
 * 3. Makes Weather Ball a power 100 Rock-type move.
 * 4. Increases the current effective evasion of pokemon with the Sand Veil ability.
 * 5. Makes Moonlight and Morning Sun restore 1/4 of the user's max HP.
 */
class SandstormEffect : public WeatherEffect
{
public:
	SandstormEffect() : WeatherEffect(5) {};
	SandstormEffect(int turns) : WeatherEffect(turns) {};
	~SandstormEffect() {};

	std::string getName() { return "Sandstorm"; };
	std::string getDescription() { return ""; };

	bool immobilises(Pokemon* p) { return false; };

	bool applyToField(BattleField* field);
	void unapplyToField(BattleField* field);

	/**
     * Does 1/16 of a Pokemon's health worth of damage each turn if the Pokemon is not 
     * Ground, Rock, or Steel type, or has the sand veil ability.
     */  
	bool tickPokemon(Pokemon* p);

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);

protected:
	void tickWeather(BattleField* field);
};
#endif