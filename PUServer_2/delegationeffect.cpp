#include "delegationeffect.h"

#include "pokemon.h"
#include "movelistentry.h"
#include "statusmove.h"
#include "pokemonmove.h"
#include "chargeeffect.h"
#include "jumpkickmove.h"
#include "raineffect.h"
#include "battlefield.h"
#include "jewelmechanics.h"

bool DelegationEffect::tick( Pokemon* p )
{
	p->removeStatus(this);
	return true;
}

MoveListEntry* DelegationEffect::getEnemyTransformedMove( Pokemon *p, MoveListEntry *entry )
{
	PokemonMove* move = entry->getMove();
	std::string name = entry->getName();

	if(name == "Feint" || name == "Shadow Force" || name == "Explosion" || name == "Selfdestruction" || isAssignableFrom<JumpKickMove*>(move)) {
		return entry;
	}

	if(!p->hasEffect<ChargeEffect>() && isAssignableFrom<StatusMove*>(move)) {
		StatusMove* smove = dynamic_cast<StatusMove*>(move);
		statusEffectList_t effects = smove->getEffects();
		for(int i = 0; i < effects.size(); ++i) {
			if(isAssignableFrom<ChargeEffect*>(effects.at(i)) ) return entry;
		}
	}

	BattleField* field = p->getField();
	if(name == "Thunder" &&
		isAssignableFrom<JewelMechanics*>(field->getMechanics()) && 
		field->getEffectByType<RainEffect>() != NULL) {
			return MoveList::getDefaultData()->getMove("Thunder");
	}

	if(move->isAttack() || name == "Roar" || name == "Whirlwind")
	{
		return new MoveListEntry(entry->getName(), new ReplaceMove(move, m_event));
	}

	return entry;
}