#include "recoilmove.h"

#include <boost/format.hpp>
#include "pokemon.h"

bool RecoilMove::isProtected( Pokemon* p )
{
	return p->hasAbility("Rock Head");
}

uint32_t RecoilMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	int health = defender->getHealth();
	int damage = mech->calculateDamage(this, attacker, defender);
	defender->changeHealth(-damage);
	health -= defender->getHealth();

	if(damage == 0)
		return 0;

	if(!isProtected(attacker))
	{
		int recoil = getRecoil(attacker, health);
		if(recoil < 1) recoil = 1;

		std::string message = boost::str(boost::format("%s was hit by recoil!") % attacker->getName());
		attacker->getField()->showMessage(message);
		attacker->changeHealth(-recoil, true);
	}

	return damage;
}