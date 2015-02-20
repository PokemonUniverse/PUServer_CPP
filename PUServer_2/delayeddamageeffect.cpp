#include "delayeddamageeffect.h"

#include "battlefield.h"
#include "pokemon.h"

bool DelayedDamageEffect::tickField( BattleField* field )
{
	if(--m_turns == 0)
	{
		if(m_damage <= 0) {
			field->showMessage("But it failed!");
			field->removeEffect(this);
			return true;
		}

		Pokemon* poke = field->getActivePokemon(m_party.m_team, m_party.m_position);
		field->showMessage(boost::str(boost::format("%s took the attack!") % poke->getName()));

		poke->changeHealth(-m_damage);
		field->removeEffect(this);

		return true;
	}

	return false;
}