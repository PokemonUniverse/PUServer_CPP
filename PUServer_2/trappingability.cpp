#include "trappingability.h"

#include "pokemon.h"
#include "statuseffect.h"

void TrappingAbility::applyToOpponent( Pokemon* owner, Pokemon* p )
{
	p->addStatus(owner, new TrappingEffectExt(this));
}

bool TrappingEffectExt::canSwitch( Pokemon* p )
{
	return !m_friend->isTrappable(p) 
		|| (p->hasAbility("Shadow Tag") && p->getOpponent()->hasAbility("Shadow Tag"))
		|| StatusEffect::canSwitch(p);
}