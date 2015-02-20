#include "criticaltypeability.h"

#include "movelistentry.h"
#include "pokemon.h"

MoveListEntry* CriticalTypeAbility::getTransformedMove(Pokemon* p, MoveListEntry* entry)
{
	double maximum = p->getStat(S_HP);
	double current = p->getHealth();
	if ((current * 3.0) > maximum) {
		return entry;
	}

	PokemonMove* move = entry->getMove();
	if (move->getType()->equals(m_type)) {
		move->setPower((int)((double)move->getPower() * 1.5));
	}

	return entry;
}