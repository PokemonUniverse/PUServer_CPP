#ifndef INC_HIDDENPOWERMOVE_H_
#define INC_HIDDENPOWERMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"
#include "pokemontype.h"

class Pokemon;
class PokemonType;

class HiddenPowerMove : public PokemonMove
{
public:
	HiddenPowerMove() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 15) {};
	~HiddenPowerMove() {};

	bool isAttack() { return true; };

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source) {
		switchIn(source);
	}

	void switchIn(Pokemon *p);
};

#endif