#ifndef INC_WEATHERMOVE_H_
#define INC_WEATHERMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

#include "weathereffect.h"

class Pokemon;
class PokemonType;
class BattleMechanics;

template < class C > 
class WeatherMove : public PokemonMove
{
	std::string m_item;

public:
	WeatherMove(PokemonType* type, int pp, std::string item)
		: PokemonMove(type, 0, 1.0, pp), m_item(item) {};
	~WeatherMove() {};

	WeatherMove* clone() { return new WeatherMove(*this); };

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int length = attacker->hasItem(m_item) ? 8 : 5;

		WeatherEffect* eff = new C;
		eff->resetTurns(length);

		attacker->getField()->applyEffect(eff);

		return 0;
	};
};
#endif