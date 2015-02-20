#include "nightmareeffect.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "sleepeffect.h"

bool NightmareEffect::apply( Pokemon* p )
{
	if(!p->hasEffect<SleepEffect>()) {
		p->getField()->showMessage("But if failed!");
		return false;
	}

	return true;
}

bool NightmareEffect::tick( Pokemon* p )
{
	if(!p->hasEffect<SleepEffect>()) {
		p->removeStatus(this);
		return true;
	}

	double maximum = (double)p->getStat(S_HP);
	int loss = (int)(maximum / 4.0);
	if(loss < 1) loss = 1;

	std::string message = boost::str(boost::format("%s is having a nightmare!") % p->getName());
	p->changeHealth(-loss);

	return false;
}