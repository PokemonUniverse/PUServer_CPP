#include "countermove.h"

#include "pokemon.h"
#include "damagelistenereffect.h"

uint32_t CounterMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	if(getEffectiveness(m_type, attacker, defender) == 0.0) {
		std::string message = boost::str(boost::format("It doesn't affect %s") % defender->getName());
		attacker->getField()->showMessage(message);
		
		return 0;
	}

	DamageListenerEffect* listener = NULL;
	statusEffectList_t effects = attacker->getNormalStatuses(0);
	statusEffectList_t::iterator it = effects.begin();
	for(; it != effects.end(); ++it)
	{
		StatusEffect* eff = (*it);
		if(isAssignableFrom<DamageListenerEffect*>(eff)) {
			listener = dynamic_cast<DamageListenerEffect*>(eff);
			break;
		}
	}

	if(listener == NULL) {
		delete listener;
		attacker->getField()->showMessage("But it failed!");
		return 0;
	}

	int damage = listener->getDamage() * 2;
	bool special = listener->isSpecial();
	bool good = (m_special == 3 || (m_special == 1 && special) || (m_special == 2 && !special));
	if(damage <= 0 || !good) {
		attacker->getField()->showMessage("But it failed!");
		return 0;
	}

	defender->changeHealth(-damage);
	return damage;
}

void CounterMove::beginTurn( battleTurnList_t turn, int index, Pokemon *source )
{
	source->addStatus(source, new DamageListenerEffect());
}