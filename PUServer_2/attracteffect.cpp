#include "attracteffect.h"

#include <boost/format.hpp>
#include "pokemon.h"
#include "coeffect.h"

bool AttractEffect::apply( Pokemon* p )
{
	int g1 = p->getGender();
	int g2 = p->getOpponent()->getGender();

	if((g1 == g2) || (g1 == GENDER_NONE || g2 == GENDER_NONE)) {
		p->getField()->showMessage("But it failed!");
		return false;
	}

	if(p->hasAbility("Oblivious")) {
		std::string message = boost::str(boost::format("%s's Oblivious prevents attraction!") % p->getName());
		p->getField()->showMessage(message);
		return false;
	}

	p->getOpponent()->addStatus(p, new CoEffect(this));
	return true;
}

bool AttractEffect::immobilises( Pokemon* p )
{
	std::string message = boost::str(boost::format("%s is in love with foe %s") % p->getName() % p->getOpponent()->getName());
	p->getField()->showMessage(message);

	if((random_range(0,100) % 2) == 0) {
		message = boost::str(boost::format("%s is immobilised by live!") % p->getName());
		p->getField()->showMessage(message);
		return true;
	}

	return false;
}