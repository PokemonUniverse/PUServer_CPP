#ifndef INC_SUNEFFECT_H_
#define INC_SUNEFFECT_H_

#include "definitions.h"
#include "weathereffect.h"

/**
 * 1. Raises the power of Fire moves by 1.5x
 * 2. Cuts the power of Water moves by 1/2.
 * 3. Makes Solarbeam a one-turn move.
 * 4. Cuts the Accuracy of Thunder to 50%.
 * 5. Doubles the current effective Speed of pokemon with the Chlorophyll  ability.
 * 6. Makes pokemon with the Forecast ability Fire-type.
 * 7. Makes Weather Ball a power 100 Fire-type move.
 * 8. Makes Moonlight and Morning Sun restore 3/4 of the user's max HP.
 * 9. Does 1/8 damage per turn to Pokemon with the Dry Skin ability.
 * 10. Does 1/8 damage per turn to Pokemon with the Solar Power ability.
 * 11. Raises the special attack of Pokemon with the Solar Power ability by 50%
 */
class SunEffect : public WeatherEffect
{
public:
	SunEffect() : WeatherEffect(5) {};
	SunEffect(int turns) : WeatherEffect(turns) {};
	~SunEffect() {};

	std::string getName() { return "Sun"; };
	std::string getDescription() { return ""; };

	bool apply(Pokemon* p);
	void unapply(Pokemon* p);

	bool immobilises(Pokemon* p) { return false; };

	bool tickPokemon(Pokemon* p);

	void unapplyToField(BattleField* field);
	bool applyToField(BattleField* field);

	MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move);

protected:
	void tickWeather(BattleField* field);

private:
	void modifySpeed(Pokemon* p, double factor);
	void modifySpAttack(Pokemon* p, double factor);
	
	void flowerGift(Pokemon* p, bool increase);
};
#endif