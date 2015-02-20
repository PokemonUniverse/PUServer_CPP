#include "flincheffect.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "statchangeeffect.h"

bool FlinchEffect::tick( Pokemon* p )
{
	if(--m_rounds == 0) {
		p->removeStatus(this);
		return true;
	}

	return false;
}

bool FlinchEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Inner Focus")) {
		return false;
	}

	return true;
}

bool FlinchEffect::immobilises( Pokemon* p )
{
	std::string message = boost::str(boost::format("%s flinched!") % p->getName());
	p->getField()->showMessage(message);
	if(p->hasAbility("Steadfast")) {
		p->addStatus(p, new StatChangeEffect(S_SPEED, true));
	}

	return true;
}