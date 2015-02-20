#ifndef INC_RAINEFFECT_H_
#define INC_RAINEFFECT_H_

#include "definitions.h"
#include "weathereffect.h"
#include "statusmove.h"

class RainEffect : public WeatherEffect
{
public:
	RainEffect() : WeatherEffect(5) {};
	RainEffect(int turns) : WeatherEffect(turns) {};
	~RainEffect() {};

	std::string getName() { return "Rain"; };
	std::string getDescription() { return ""; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool immobilises(Pokemon* p) { return false; };

	bool tickPokemon(Pokemon* p);

	bool applyToField(BattleField* field);
	void unapplyToField(BattleField* field);

	/**
     * 1. Raises the power of Water moves by 1.5x
     * 2. Cuts the power of Fire moves by 1/2.
     * 3. Cuts the power of Solarbeam to 60.
     * 4. Makes Thunder have perfect accuracy.  (Perfect, not 100%)
     * 5. Doubles the current effective Speed of pokemon with the Swift Swim ability.
     * 6. Restores 1/16 HP to pokemon with the Rain Dish ability.
     * 7. Makes pokemon with the Forecast ability Water-type.
     * 8. Makes Weather Ball a power 100 Water-type move.
     * 9. Makes Moonlight and Morning Sun restore 1/4 of the user's max HP.
     */	
	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);

protected:
	void tickWeather(BattleField* field);

private:
	void modifySpeed(Pokemon* p, double factor);

};

class RainEffectThunder : public StatusMove
{
public:
	RainEffectThunder(statusEffectList_t statuses, boolArray_t attacker, doubleArray_t chances) 
		: StatusMove(PokemonType::T_ELECTRIC, 120, 0.7, 10, statuses, attacker, chances) {};
	~RainEffectThunder() {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif