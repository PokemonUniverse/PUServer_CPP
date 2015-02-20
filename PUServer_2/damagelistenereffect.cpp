#include "damagelistenereffect.h"

#include "pokemon.h"
#include "movelistentry.h"

bool DamageListenerEffect::tick( Pokemon* p )
{
	p->removeStatus(this);
	return true;
}

void DamageListenerEffect::informDamaged( Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage )
{
	m_damage += damage;
	m_special = move->getMove()->isSpecial(attacker->getField()->getMechanics());
}