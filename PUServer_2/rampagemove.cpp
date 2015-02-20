#include "rampagemove.h"

#include "pokemon.h"
#include "confuseeffect.h"

#include <boost/format.hpp>

bool RampageMove::RampageStatusEffect::tick( Pokemon* p )
{
	if(--m_turns == 0)
	{
		std::string message = boost::str(boost::format("%s's rampage ended!") % p->getName());
		p->getField()->showMessage(message);
		p->removeStatus(this);
		p->addStatus(p, new ConfuseEffect());
	}

	return true;
}

bool RampageMove::RampageStatusEffect::apply( Pokemon* p )
{
	m_turns = random_range(2,3);
	return true;
}

uint32_t RampageMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	attacker->addStatus(attacker, new RampageStatusEffect(this));

	return PokemonMove::use(mech, attacker, defender);
}