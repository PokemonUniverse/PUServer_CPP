#ifndef INC_FIXEDATTACKMOVE_H_
#define INC_FIXEDATTACKMOVE_H_

#include "definitions.h"
#include "statusmove.h"

/**
* A move that prevents the user from using other moves until the move misses
* or the number of turns is reached.
*/
class FixedAttackMove : public StatusMove
{
public:
	FixedAttackMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp, FixedAttackEffect* eff)
	{
		statusEffectList_t a1;
		a1.push_back(eff);
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(type, power, accuracy, pp, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		if(StatusMove::attemptHit(mech, attacker, defender)) {
			return StatusMove::use(mech, attacker, defender);
		}

		attacker->removeStatus<FixedAttackEffect>();
		return 0;
	}
};
#endif