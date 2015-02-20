#include "chargeeffect.h"

#include "pokemon.h"
#include "movelistentry.h"
#include "battlefield.h"

MoveListEntry* ChargeEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(m_turns == 0) {
		p->removeStatus(this);
		return m_move;
	}

	p->getField()->showMessage(boost::str(boost::format("%1% %2%") % p->getName() % m_msg));
	return NULL;
}

bool ChargeEffect::apply( Pokemon* p )
{
	if(m_turns != 0) {
		p->getField()->showMessage(boost::str(boost::format("%1% %2%") % p->getName() % m_msg));
		return true;
	}

	p->useMove(m_move->getMove(), p->getOpponent());
	return false;
}