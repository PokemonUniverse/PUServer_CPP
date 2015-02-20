#ifndef __WEATHEREFFECT_H_
#define __WEATHEREFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

#include <boost/unordered_map.hpp>
#include "pokemonspecies.h"

class Pokemon;
class BattleField;

class WeatherEffect : public FieldEffect
{
public:
    /**
     * Initialise the number of turns that this effect will remain active
     * for before it fades.
     */
	WeatherEffect(int turns) {
		m_turns = turns;
		m_lock = WEATHER_EFFECT_LOCK; 
	};
	virtual ~WeatherEffect() {};

    /**
     * Weather effects are currently all in the third tier (zero is the first
     * tier, so returning two is indeed the third tier).
     */
	int getTier() { return 2; };

	virtual bool isMoveTransformer(bool enemy) { return !enemy;	};

	void resetTurns(int turns) { m_turns = turns; };

protected:
    /**
     * Tick the weather on this field.
     */
	virtual void tickWeather(BattleField* field) {};

    /**
     * Handle the number of turns that this effect will last for. This method
     * actually deactivatves the effect if it is time for it to fade.
     * Subclasses should return super.tickField()!
     * @return whether the weather effect faded
     */
	bool tickField(BattleField* field);

    /**
     * Whether the weather should have effects.
     */
	bool hasEffects(BattleField* field);

    /**
     * Changes the types of a Pokemon (for Forecast).
     * @param apply Whether the types are being applied or unapplied.
     */
	void setTypes(Pokemon* p, pokemonTypeList_t types, bool apply);

	typedef boost::unordered_map<teamInfo_s, bool> appliedList_t;
	appliedList_t m_applied;

private:
	int m_turns;

	/** Stores types for Forecast
	*  Note: The same effect is applied to all pokemon on the field! You cannot
	*        do this naively with just a single array!
	*/
	typedef boost::unordered_map<teamInfo_s, pokemonTypeList_t> pokemonTypeHolder_t;
	pokemonTypeHolder_t m_types;
};

#endif