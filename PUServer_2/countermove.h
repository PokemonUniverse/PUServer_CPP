#ifndef INC_COUNTERMOVE_H_
#define INC_COUNTERMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class PokemonType;
class BattleMechanics;

class CounterMove : public PokemonMove
{
	int m_special; // 1 is special, 2 is physical, 3 is both

public:
	CounterMove(PokemonType* type, double accuracy, uint32_t pp, int special)
		: PokemonMove(type, 0, accuracy, pp), m_special(special) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source);

	bool isDamaging() { return true; };
	bool isAttack() { return true; };

	int getPriority() { return -4; };
};

#endif