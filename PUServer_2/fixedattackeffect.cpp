#include "fixedattackeffect.h"

#include "pokemon.h"

int FixedAttackEffect::getMoveIndex( Pokemon *p )
{
	for(int32_t i = 0; i < 4; ++i) 
	{
		MoveListEntry *move = p->getMove(i);
		if (move == NULL)
			continue;

		if (move->getName() == getName()) {
			return i;
		}
	}
	return -1;
}

bool FixedAttackEffect::apply( Pokemon* p )
{
	m_turns = random_range(2,6);
	int idx = getMoveIndex(p);
	m_pp = p->getPp(idx);

	return StatusEffect::apply(p);
}

bool FixedAttackEffect::tick( Pokemon* p )
{
	int idx = getMoveIndex(p);
	p->setPp(idx, m_pp);

	if (m_turns-- <= 0) {
		if (m_message.length() > 0) {
			p->getField()->showMessage(p->getName() + m_message);
		}

		p->removeStatus(this);
		return true;
	}

	return false;                            
}

bool FixedAttackEffect::vetoesMove( Pokemon* p, MoveListEntry* entry )
{
	return !(entry->getName() == getName());
}

void FixedAttackEffect::executeTurn( Pokemon* p, BattleTurn* turn )
{
	m_pp = p->getPp(turn->getId());
}