#ifndef __BATTLEMECHANICS_H_
#define __BATTLEMECHANICS_H_

#include "definitions.h"
#include "tools.h"

class Pokemon;
class PokemonMove;

class BattleMechanics
{
public:
	BattleMechanics() {};
	virtual ~BattleMechanics() {};

	virtual uint32_t calculateStat(Pokemon *p, int i) = 0;
	virtual uint32_t calculateDamage(PokemonMove *move, Pokemon *attacker, Pokemon *defender, bool silent = false) = 0;

	virtual void validateHiddenStats(Pokemon *p) = 0;

	virtual bool attemptHit(PokemonMove *move, Pokemon *attacker, Pokemon *defender) = 0;
	virtual bool isMoveSpecial(PokemonMove *move) = 0;
};

#endif