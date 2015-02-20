#include "perishsongeffect.h"

#include "pokemon.h"

bool PerishSongEffect::apply( Pokemon* p )
{
	// No need to reference Cacophony here
	return !(m_soundImmune && p->hasAbility("Soundproof"));
}

bool PerishSongEffect::tick( Pokemon* p )
{
	std::string message = boost::str(boost::format("%s's perish count fell too %d") % p->getName() % m_turns);
	p->getField()->showMessage(message);

	if(m_turns-- == 0) {
		p->faint();
	}

	return true;
}