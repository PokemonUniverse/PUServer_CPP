#include "trappingeffect.h"

#include "pokemon.h"
#include "coeffect.h"

bool TrappingEffect::apply( Pokemon* p )
{
	p->getOpponent()->addStatus(p, new CoEffect(this));
	return StatusEffect::apply(p);
}

bool TrappingEffect::canSwitch( Pokemon* p )
{
	return p->hasItem("Shed Shell");
}