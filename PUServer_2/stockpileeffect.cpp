#include "stockpileeffect.h"

#include "pokemon.h"
#include "statchangeeffect.h"
#include "jewelmechanics.h"

bool StockpileEffect::incrementLevel( Pokemon* p )
{
	std::string message;
	if(m_levels < 3) {
		m_levels++;

		message = boost::str(boost::format("%s stockpiled %s!") % p->getName() % m_levels);
		p->getField()->showMessage(message);

		if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics())) {
			m_effects.push_back(p->addStatus(p, new StatChangeEffect(S_DEFENCE, true)));
			m_effects.push_back(p->addStatus(p, new StatChangeEffect(S_SPDEFENCE, true)));
		}

		return true;
	}

	message = boost::str(boost::format("%s couldn't stockpile any more!") % p->getName());
	p->getField()->showMessage(message);

	return false;
}

void StockpileEffect::unapply( Pokemon* p )
{
	statusEffectList_t::iterator it = m_effects.begin();
	for(; it != m_effects.end(); ++it) {
		p->removeStatus((*it));
	}

	StatusEffect::unapply(p);
}