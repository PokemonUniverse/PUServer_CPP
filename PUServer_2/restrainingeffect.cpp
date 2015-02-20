#include "restrainingeffect.h"

#include "pokemon.h"
#include "coeffect.h"
#include "tools.h"

bool RestrainingEffect::tick( Pokemon* p )
{
	std::string message;
	if(m_turns-- <= 0) {
		message = boost::str(boost::format("%s was released from %s!") % p->getName() % m_description);
		p->getField()->showMessage(message);
		p->removeStatus(this);

		return false;
	}

	int maximum = p->getStat(S_HP);
	int damage = maximum / 16;
	if(damage <= 0) damage = 1;

	message = boost::str(boost::format("%s is hurt by %s!") % p->getName() % m_name);
	p->changeHealth(-damage);

	return true;
}

bool RestrainingEffect::apply( Pokemon* p )
{
	if(p->hasEffect<RestrainingEffect>()) {
		p->removeStatus<RestrainingEffect>();
	}

	m_turns = random_range(1,4);
	p->getOpponent()->addStatus(p, new CoEffect(this));

	return true;
}

bool RestrainingEffect::canSwitch( Pokemon* p )
{
	return p->hasItem("Shed Shell");
}