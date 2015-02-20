#include "opponenteffectability.h"

#include "pokemon.h"
#include "statuseffect.h"

void OpponentEffectAbility::unapply( Pokemon* p )
{
	if(m_effect != NULL) {
		p->getField()->removeEffect(m_effect);
	}
}

bool OpponentEffectAbility::switchOut( Pokemon* p )
{
	unapply(p);
	return IntrinsicAbility::switchOut(p);
}

void OpponentEffectAbility::switchIn( Pokemon *p )
{
	m_effect = new OpponentFieldEffect(p, this);
	p->getField()->applyEffect(m_effect);
}

bool OpponentFieldEffect::equals( StatusEffect* eff )
{
	if(!isAssignableFrom<OpponentFieldEffect*>(eff))
		return false;

	OpponentFieldEffect* e2 = dynamic_cast<OpponentFieldEffect*>(eff);
	
	return (m_owner->getGUID() == e2->getOwner()->getGUID());
}

bool OpponentFieldEffect::apply( Pokemon* p )
{
	if(m_owner->getGUID() != p->getGUID()) {
		m_friend->applyToOpponent(m_owner, p);
	}

	return true;
}