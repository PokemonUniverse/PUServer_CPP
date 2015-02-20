#include "invulnerablestateeffect.h"

#include "pokemon.h"
#include "movelistentry.h"
#include "pokemonmove.h"

MoveListEntry* InvulnerableStateEffect::getEnemyTransformedMove( Pokemon *p, MoveListEntry *move )
{
	std::string moveName = move->getName();
	PokemonMove* pmove = move->getMove();

	for(std::vector<std::string>::iterator it = m_effectiveMoves.begin(); it != m_effectiveMoves.end(); ++it) {
		if(moveName == (*it)) {
			pmove->setPower(pmove->getPower() * 2);
			return move;
		}
	}

	if(!p->hasAbility("No Guard") && !p->getOpponent()->hasAbility("No Guard")) {
		pmove->setAccuracy(0.0);
	}

	return move;
}

bool InvulnerableStateEffect::immobilises( Pokemon* p )
{
	p->removeStatus(this);
	return false;
}