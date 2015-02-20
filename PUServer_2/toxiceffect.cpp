#include "toxiceffect.h"

#include <boost/format.hpp>
#include "pokemon.h"

bool ToxicEffect::tick( Pokemon* p )
{
	int damage;
	std::string message;

	if(p->hasAbility("Poison Heal")) {
		damage = (int)(p->getStat(S_HP) / 8);
		message = boost::str(boost::format("%s's Poison Heal restored health!") % p->getName());
		p->getField()->showMessage(message);
		p->changeHealth(damage, true);
	}
	else {
		damage = (int)((p->getStat(S_HP) * m_turns) / 16);
		if(m_turns < 8) { ++m_turns; }
		if(damage <= 0) { damage = 1; }

		message = boost::str(boost::format("%s is hurt by poison!") % p->getName());
		p->getField()->showMessage(message);
		p->changeHealth(-damage, true);
	}

	return false;
}

bool ToxicEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Immunity")) { return false; }
	if(p->isType(PokemonType::T_POISON) || p->isType(PokemonType::T_STEEL)) { return false; }

	m_turns = 1;
	return true;
}