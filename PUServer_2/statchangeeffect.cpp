#include "statchangeeffect.h"

#include <boost/format.hpp>

#include "pokemon.h"
#include "statmultiplier.h"

std::string StatChangeEffect::getDefaultDescription()
{
	std::string adj = "";
	std::string adv = "";

	if(m_raise) {
		adj = "raised";
		if(m_stages == 2) {
			adv = "sharply";
		}
	}
	else {
		adj = "lowered";
		if(m_stages == 2) {
			adv = "harshly";
		}
	}

	return boost::str(boost::format("'s %s was %s %s.") % Pokemon::getStatName(m_stat) % adv % adj);
}

StatMultiplier* StatChangeEffect::getMultiplier( Pokemon* p )
{
	StatMultiplier* mul = NULL;
	if(m_stat == S_ACCURACY) {
		mul = p->getAccuracy();
	} else if(m_stat == S_EVASION) {
		mul = p->getEvasion();
	} else {
		mul = p->getMultiplier(m_stat);
	}

	return mul;
}

bool StatChangeEffect::apply( Pokemon* p )
{
	if(getInducer() != p) {
		if(!m_raise && (p->hasAbility("Clear Body") || p->hasAbility("White Smoke"))) {
			return false;
		}

		if(!m_raise) {
			//TODO: Uncomment when MistEffect & PartyEffect are implemented
			/*
			MistEffect* effect = (MistEffect*)p->getField()->getEffectByType<MistEffect>();
			if(effect != NULL) {
				if(effect.isActive(/ *get pkmn slot here* /)) { return false; }
			}*/

		}

		if(!m_raise && (m_stat == S_ATTACK) && p->hasAbility("Hyper Cutter")) {
			return false;
		}
		if(!m_raise && (m_stat == S_ACCURACY) && p->hasAbility("Keen Eye")) {
			return false;
		}
	}

	StatMultiplier* mul = getMultiplier(p);
	if(!mul) { 
		return false;
	}

	int total = 0;
	for(int i = 0; i < m_stages; ++i) {
		bool changed = false;
		if(m_raise) {
			changed = mul->increaseMultiplier();
		}
		else {
			changed = mul->decreaseMultiplier();
		}

		if(changed) {
			++total;
		}
	}

	m_undo = total;
	if(total == 0) {
		std::string keyword = m_raise ? "higher" : "lower";
		std::string message = boost::str(boost::format("'s %s won't go %s!") % Pokemon::getStatName(m_stat) % keyword);
		p->getField()->showMessage(message);

		return false;
	}

	return true;
}

void StatChangeEffect::unapply( Pokemon* p )
{
	StatMultiplier* mul = getMultiplier(p);
	for(int i = 0; i < m_undo; ++i) {
		if(m_raise) {
			mul->decreaseMultiplier();
		} else {
			mul->increaseMultiplier();
		}
	}
}