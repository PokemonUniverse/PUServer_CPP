#include "sleepeffect.h"

#include "fixedattackeffect.h"
#include "pokemon.h"

SleepEffect* SleepEffect::m_dummy = new SleepEffect();

bool SleepEffect::apply( Pokemon* p )
{
	if(p->hasAbility("Insomnia") || p->hasAbility("Vital Spirit")) {
		return false;
	}

	if(p->hasEffect<FixedAttackEffect>()) {
		StatusEffect *effect = p->getEffect<FixedAttackEffect>();
		if(effect->getName() == "Uproar") {
			p->getField()->showMessage("But it failed!");
			return false;
		}
	}

	if(m_turns == 0) {
		m_turns = random_range(2,6);
	}
	if(p->hasAbility("Early Bird")) {
		m_turns = (int)(m_turns / 2) + 1;
	}

	return true;
}

bool SleepEffect::immobilises( Pokemon* p )
{
	std::string message;
	if(--m_turns <= 0) {
		p->removeStatus(this);
		message = boost::str(boost::format("%s woke up!") % p->getName());
		p->getField()->showMessage(message);
		return false;
	}

	message = boost::str(boost::format("%s is fast asleep!") % p->getName());
	p->getField()->showMessage(message);

	return true;
}