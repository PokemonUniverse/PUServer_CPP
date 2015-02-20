#include "delayedstatuseffect.h"

#include "battlefield.h"
#include "pokemon.h"

bool DelayedStatusEffect::applyToField( BattleField* field )
{
	field->showMessage(boost::str(boost::format("%s %s") % field->getActivePokemon(m_party.m_team, m_party.m_position)->getName() % m_message));
	return true;
}

bool DelayedStatusEffect::tickField( BattleField* field )
{
	if(--m_turns == 0) {
		Pokemon* poke = field->getActivePokemon(m_party.m_team, m_party.m_position);
		poke->addStatus(poke->getOpponent(), m_effect);
		field->removeEffect(this);

		return true;
	}

	return false;
}