#include "leechseedeffect.h"

#include "pokemon.h"

bool LeechSeedEffect::tick( Pokemon* p )
{
	Pokemon* opponent = p->getOpponent();
	if(p->isFainted() || opponent->isFainted()) return false;

	int damage = p->getStat(S_HP) / 8;
	if(damage == 0) damage = 1;

	if(!p->hasAbility("Magic Guard")) {
		std::string message = boost::str(boost::format("%s's health was sapped by leech seed!") % p->getName());
		p->changeHealth(-damage, true);

		if(p->hasAbility("Liquid Ooze")) {
			message = boost::str(boost::format("%s sucked up liquid ooze!") % opponent->getName());
			p->getField()->showMessage(message);
			opponent->changeHealth(-damage);
		} else {
			message = boost::str(boost::format("%s regained health!") % opponent->getName());
			p->getField()->showMessage(message);
			opponent->changeHealth(damage);
		}
	}

	return false;
}

bool LeechSeedEffect::apply( Pokemon* p )
{
	if(m_grassImmune && p->isType(PokemonType::T_GRASS)) {
		return false;
	}

	return true;
}