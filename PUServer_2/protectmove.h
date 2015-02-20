#ifndef INC_PROTECTMOVE_H_
#define INC_PROTECTMOVE_H_

#include "definitions.h"
#include "statusmove.h"

class BattleMechanics;
class Pokemon;
class DelegationEffect;
class PokemonType;

class ProtectMove : public StatusMove
{
	double m_failure;

public:
	ProtectMove(PokemonType* type, uint32_t pp, DelegationEffect* effect);;

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

	void setFailure(double failure) { m_failure = failure; };
	int getPriority() { return 2; };
	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};
#endif