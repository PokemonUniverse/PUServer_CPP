#include "partyeffect.h"

#include "battlefield.h"
#include "pokemon.h"

#include <boost/format.hpp>

bool PartyEffect::tickField( BattleField* field )
{
	bool stillActive = false;
	for(int i = 0; i < 2; ++i)
	{
		if(!m_active[i]) continue;
		if(--m_turns[i] <= 0) {
			if(m_endMessage.length() > 0) {
				std::string message = boost::str(boost::format("%1%%2%") % field->getActivePokemon(i)->getName() % m_endMessage);
				field->showMessage(message);
			}

			m_active[i] = false;
		}
		
		if(m_active[i]) stillActive = true;
	}

	if(!stillActive) {
		field->removeEffect(this);
		return true;
	}

	return false;
}

void PartyEffect::activateParty( Pokemon* p )
{
	BattleField* field = p->getField();
	uint8_t party = p->getMaster()->getBattleTeamIdx();

	if(m_active[party]) {
		informDuplicateEffect(p);
		return;
	}

	m_active[party] = true;
	m_turns[party] = 5;

	if(m_startMessage.length() > 0) {
		std::string message = boost::str(boost::format("%1%%2%") % field->getActivePokemon(party)->getName() % m_startMessage);
		field->showMessage(message);
	}
}