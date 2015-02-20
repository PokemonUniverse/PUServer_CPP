#include "ingraineffect.h"

#include <boost/format.hpp>

#include "pokemon.h"

bool IngrainEffect::tick( Pokemon* p )
{
	int absorb = p->getStat(S_HP) / 16;
	if(absorb == 0) absorb = 1;

	std::string message = boost::str(boost::format("%s absorbed health!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(absorb);

	return true;
}

bool IngrainEffect::canSwitch( Pokemon* p )
{
	return p->hasItem("Shed Shell");
}