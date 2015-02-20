#include "statusmove.h"

#include <boost/format.hpp>

#include "statuseffect.h"
#include "sleepeffect.h"
#include "pokemon.h"

bool StatusMove::isEffective( Pokemon* target )
{
	for(int i = 0; i < m_statuses.size(); ++i)
	{
		StatusEffect* status = m_statuses.at(i);

		// Exeption 1: Non-special effects ignore type immunities.
		if(status == NULL || status->getLock() != SPECIAL_EFFECT_LOCK)
			continue;

		// Exeption 2: Sleep ignores type immunites.
		if(isAssignableFrom<SleepEffect*>(status))
			continue;

		// Exeption 3: Wonder Guard pokemon ignore type immunities.
		if(target->hasAbility("Wonder Guard"))
			continue;

		return false;
	}

	return true;
}

uint32_t StatusMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	int damage = 0;
	bool hasSubstitute = defender->hasSubstitute();
	const bool ineffective = (getEffectiveness(attacker, defender) == 0.0);

	if(m_power != 0) {
		damage = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-damage);
		if(ineffective) {
			return 0;
		}
	}
	else if(!m_personal && ineffective && !isEffective(defender)) {
		std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
		attacker->getField()->showMessage(message);

		return 0;
	}

	const bool serene = attacker->hasAbility("Serene Grace");
	const bool immune = (defender->hasAbility("Shield Dust") && (m_power != 0) );

	for(int i = 0; i < m_statuses.size(); ++i)
	{
		if(!m_attacker.at(i) && immune) {
			continue;
		}
		double chance = m_chances.at(i);
		if(serene) {
			chance *= 2.0;
		}

		if(random_range(0, 100) <= (chance * 100)) {
			Pokemon* affected = (m_attacker.at(i) ? attacker : defender);
			if((attacker != affected) && hasSubstitute && !m_statuses.at(i)->hitsThroughSubstitude()) {
				if(m_power == 0) {
					attacker->getField()->showMessage("But it failed!");
				}
				continue;
			}

			if(affected->addStatus(attacker, m_statuses.at(i)) == NULL) {
				if(m_power == 0) {
					// Only show the message if it is a primary effect
					m_statuses.at(i)->informDuplicateEffect(affected);
				}
			}
		}
	}

	return damage;
}