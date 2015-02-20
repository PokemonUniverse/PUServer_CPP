#ifndef __ADVANCEDMECHANICS_H_
#define __ADVANCEDMECHANICS_H_

#include "definitions.h"
#include "battlemechanics.h"

class Pokemon;
class PokemonMove;

class AdvancedMechanics : public BattleMechanics
{
public:
	AdvancedMechanics() {};
	~AdvancedMechanics() {};

	virtual uint32_t calculateStat(Pokemon *p, int i);
	virtual uint32_t calculateDamage(PokemonMove *move, Pokemon *attacker, Pokemon *defender, bool silent = false);

	virtual bool attemptHit(PokemonMove *move, Pokemon *attacker, Pokemon *defender);
	virtual bool isMoveSpecial(PokemonMove *move);

	bool isCriticalHit(PokemonMove* move, Pokemon* user, Pokemon* target);

private:
	static int m_log;
};
#endif