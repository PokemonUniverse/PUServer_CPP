#ifndef INC_STATCHANGESWAPMOVE_H_
#define INC_STATCHANGESWAPMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

#include "statusmove.h"

class BattleMechanics;
class Pokemon;
class PokemonType;

class StatChangeSwapMove : public PokemonMove
{
	intArray_t m_stats;

public:
	StatChangeSwapMove(PokemonType* type, uint32_t pp, intArray_t stats) : PokemonMove(type, 0, 1.0, pp), m_stats(stats) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	}

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

protected:
	StatChangeSwapMove() {};

private:
	statusEffectList_t cleanList(statusEffectList_t list);

	void addStatuses(Pokemon *p, statusEffectList_t effects);
	void removeStatuses(Pokemon *p, statusEffectList_t effects);
};
#endif