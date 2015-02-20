#ifndef INC_ABSORBMOVE_H_
#define INC_ABSORBMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class BattleMechanics;
class PokemonType;

class AbsorbMove : public PokemonMove
{
	double m_percent;

public:
	AbsorbMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp, double percent) 
		: PokemonMove(type, power, accuracy, pp), m_percent(percent) {};
	~AbsorbMove() {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

};
#endif