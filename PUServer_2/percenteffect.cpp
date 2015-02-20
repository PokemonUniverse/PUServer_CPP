#include "percenteffect.h"

#include <boost/format.hpp>
#include "pokemon.h"

bool PercentEffect::tick( Pokemon* p )
{
	/* TODO: Uncomment when HealBlockEffect is implemented
	if(p->hasEffect<HealBlockEffect>()) {
		return false;
	}*/

	std::string message = boost::str(boost::format("%s %s") % p->getName() % m_description);
	p->getField()->showMessage(message);
	p->changeHealth(calculateChange(p, m_percent), true);

	return false;
}

bool PercentEffect::apply( Pokemon* p )
{
	if(m_tier != -1) { return true; };

	/* TODO: Uncomment when HealBlockEffect is implemented (2)
	if(p->hasEffect<HealBlockEffect>()) { return false; };*/

	if(m_description.length() > 0) {
		p->getField()->showMessage(m_description);
	}

	p->changeHealth(calculateChange(p, m_percent), true);

	return false;
}

int PercentEffect::calculateChange( Pokemon* p, double percent )
{
	double maximum = (double)p->getStat(S_HP);
	double change = maximum * m_percent;
	int flr = (int)floor(change);

	return flr;
}