#ifndef INC_HEALTHBASEDMOVE_H_
#define INC_HEALTHBASEDMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class PokemonType;
class BattleMechanics;

class HealthBasedMove : public PokemonMove
{
	bool m_highHp;

public:
	HealthBasedMove(PokemonType* type, double accuracy, uint32_t pp, bool highHp)
		: PokemonMove(type, 0, accuracy, pp), m_highHp(highHp) {};
	~HealthBasedMove() {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif