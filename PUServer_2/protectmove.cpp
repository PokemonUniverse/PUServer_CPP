#include "protectmove.h"

#include "countereffect.h"
#include "battlemechanics.h"
#include "delegationeffect.h"
#include "pokemon.h"
#include "tools.h"

ProtectMove::ProtectMove( PokemonType* type, uint32_t pp, DelegationEffect* effect )
{
	statusEffectList_t a1;
	a1.push_back(effect);
	a1.push_back(new CounterEffect());
	boolArray_t a2(2, true);
	doubleArray_t a3(2, 1.0);

	StatusMove(type, 0, 1.0, pp, a1, a2, a3);
}

uint32_t ProtectMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	if(random_range(0, 100) < (m_failure*100)) {
		attacker->getField()->showMessage("But it failed!");
		attacker->removeStatus<CounterEffect>();
		return 0;
	}

	return StatusMove::use(mech, attacker, defender);
}