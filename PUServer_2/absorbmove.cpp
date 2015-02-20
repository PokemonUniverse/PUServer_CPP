#include "absorbmove.h"

#include "battlemechanics.h"
#include "pokemon.h"

uint32_t AbsorbMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	int damage = mech->calculateDamage(this, attacker, defender);
	int health = defender->getHealth();
	defender->changeHealth(-damage);
	if(damage > health) {
		damage = health;
	}

	int absorb = (int)((double)damage * m_percent);
	if(absorb < 1) absorb = 1;
	
	std::string message;

	if(defender->hasAbility("Liquid Ooze")) {
		absorb = -absorb;
		message = boost::str(boost::format("%s sucked up liquid ooze!") % attacker->getName());
	}
	else {
		message = boost::str(boost::format("%s absorbed health!") % attacker->getName());
	}

	attacker->getField()->showMessage(message);
	attacker->changeHealth(absorb);

	return damage;
}