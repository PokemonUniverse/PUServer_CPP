#include "healthbasedmove.h"

#include "battlemechanics.h"
#include "pokemon.h"

uint32_t HealthBasedMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	double healthPercent = ((double)attacker->getHealth() / (double)attacker->getStat(S_HP)) * 100.0;

	int power = 0;
	if(m_highHp) {
		power = (int)(healthPercent * 1.5);
	} else if(healthPercent <= 4.1) {
		power = 200;
	} else if(healthPercent <= 10.4) {
		power = 150;
	} else if(healthPercent <= 20.8) {
		power = 100;
	} else if(healthPercent <= 35.4) {
		power = 80;
	} else if(healthPercent <= 68.7) {
		power = 40;
	} else {
		power = 20;
	}

	setPower(power);
	int damage = mech->calculateDamage(this, attacker, defender);
	defender->changeHealth(-damage);

	return damage;
}