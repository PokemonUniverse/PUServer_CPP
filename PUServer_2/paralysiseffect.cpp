#include "paralysiseffect.h"

#include "pokemon.h"

bool ParalysisEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Limber")) {
		return false;
	}

	if(!p->hasAbility("Quick Feet")) {
		p->getMultiplier(S_SPEED)->divideBy(4.0);
	}

	return true;
}

void ParalysisEffect::unapply( Pokemon* p )
{
	if(!p->hasAbility("Quick Feet")) {
		p->getMultiplier(S_SPEED)->multiplyBy(4.0);
	}
}

bool ParalysisEffect::immobilises( Pokemon* p )
{
	int rnd = random_range(0,100);
	if(rnd <= 25) {
		std::string message = boost::str(boost::format("%s is paralysed! It can't move!") % p->getName());
		p->getField()->showMessage(message);

		return true;
	}

	return false;
}