#include "statuseffect.h"
#include "fieldeffect.h"
#include "creature.h"
#include "pokemon.h"
#include "pokemontype.h"
#include "pokemonparty.h"

#include "battlefield.h"
#include "pokemonwrapper.h"
#include "battleteam.h"

pokemonList_t BattleField::getActivePokemon()
{
	pokemonList_t activeList;

	if(m_creatures.size() == 0 || m_creatures.at(0) == NULL || m_creatures.at(1) == NULL)
		return activeList;

	BattleTeam* team;
	for(int i = 0; i < 2; i++) {
		team = m_creatures.at(0);
		pokemonList_t list = team->getActivePokemonList();
		foreach(Pokemon* p, list) {
			activeList.push_back(p);
		}
	}
	
	return activeList;
}

Pokemon* BattleField::getActivePokemon( uint32_t team )
{
	BattleTeam* teamPtr = m_creatures.at(team);
	return teamPtr->getActivePokemon();
}

Pokemon* BattleField::getActivePokemon( uint32_t team, uint32_t idx) 
{
	if(idx == -1) return getActivePokemon(team);

	BattleTeam* battleTeam = m_creatures.at(team);
	return battleTeam->getActivePokemon(idx);
}

Pokemon* BattleField::getActivePokemon( Creature* creature )
{
	return creature->getPokemonParty()->getActivePokemon();
}

void BattleField::setPokemon( battleTeamList_t teams )
{
	m_creatures = teams;
	pokemonList_t active = getActivePokemon();
	sortBySpeed(&active);

	pokemonList_t::iterator it;
	foreach(Pokemon* p, active)
	{
		applyEffects(p);
		p->switchIn();
	}
}

void BattleField::sortBySpeed( pokemonList_t* pokemon )
{
	std::sort(pokemon->begin(), pokemon->end(), PokemonWrapper::compareSpeed);
}

double BattleField::getEffectiveness( PokemonType *move, PokemonType *pokemon, bool enemy )
{
	return Pokemon::getEffectiveness(m_effects, move, pokemon, enemy);
}

void BattleField::applyEffects( Pokemon *p )
{
	fieldEffects_t::iterator it;
	for(it = m_effects.begin(); it != m_effects.end(); ++it)
	{
		FieldEffect *eff = (*it);
		if(!eff->isRemovable()) {
			p->addStatus(NULL, eff);
		}
	}
}

bool BattleField::applyEffect( FieldEffect* eff )
{
	fieldEffects_t::iterator it;
	for(it = m_effects.begin(); it != m_effects.end(); ++it)
	{
		FieldEffect* j = (*it);
		if(j->isRemovable()) continue;
		if(eff->equals(j)) return false;

		if(eff->isExclusiveWith(j)) {
			// FieldEffects overwrite each other rather than failing if
			// another in their class is present.
			removeEffect(j);
			// We know that no other statuses can possibly be in this
			// category, so it is safe to skip the rest of this loop.
			break;
		}

		FieldEffect* applied = eff->getFieldCopy();
		if(!applied->applyToField(this)) return false;

		m_effects.push_back(applied);

		// Apply to each pokemon in the field
		pokemonList_t active = getActivePokemon();
		for(pokemonList_t::iterator it = active.begin(); it != active.end(); ++it)
		{
			Pokemon* pokemon = (*it);
			pokemon->addStatus(NULL, applied);
		}
	}

	return true;
}

void BattleField::removeEffect( FieldEffect* eff )
{
	pokemonList_t active = getActivePokemon();
	for(pokemonList_t::iterator it = active.begin(); it != active.end(); ++it)
	{
		Pokemon* pokemon = (*it);
		eff->unapply(pokemon);
	}

	eff->unapplyToField(this);
	eff->disable();
}

void BattleField::synchroniseFieldEffects()
{
	fieldEffects_t::iterator it = m_effects.begin();
	while(it != m_effects.end())
	{
		StatusEffect* eff = (StatusEffect*)(*it);
		if(eff->isRemovable()) {
			it = m_effects.erase(it);
			delete eff;

			continue;
		}

		++it;
	}
}

void BattleField::switchInPokemon( Creature *trainer, Pokemon *p )
{
	trainer->getPokemonParty()->getActivePokemon()->switchOut();

	informSwitchInPokemon(trainer, p);
	applyEffects(p);
	p->switchIn();
}

void BattleField::replaceFaintedPokemon( Creature *trainer, Pokemon *pokemon, bool search )
{
	switchInPokemon(trainer, pokemon);
	if(!search)
		return;

	foreach(BattleTeam* team, m_creatures)
	{
		pokemonList_t activePokemon = team->getActivePokemonList();
		foreach(Pokemon* p, activePokemon) {
			if(p->isFainted()) {
				requestPokemonReplacement(p->getMaster());
				return; //TODO: Not sure but this might block if multiple pkmn faint.
			}
		}
	}

	requestsMoves();
}

void BattleField::executeTurn( BattleTurn *turn, Pokemon *source, Pokemon *target )
{
	if(source->isFainted()) {
		return;
	}

	if(!turn->isMoveTurn()) {
		Creature *trainer = source->getMaster();
		Pokemon *p = trainer->getPokemonParty()->get(turn->getId());
		switchInPokemon(trainer, p);
		
		return;
	}

	source->executeTurn(turn);
	
	int32_t move = turn->getId();
	MoveListEntry *entry = source->getMove(move);
	if(!entry) {
		return;
	}
	PokemonMove *theMove = entry->getMove();

	if(source->isImmobilised(theMove->getStatusException())) {
		return;
	}
	
	if(theMove->isAttack() && target->isFainted()) {
		informUseMove(source, entry->getName());
		showMessage("But there was no target!");

		return;
	}

	source->useMove(move, target);
}

void BattleField::executeTurn( battleTurnList_t move )
{
	pokemonList_t active = getActivePokemon();
	PokemonWrapper::sortMoves(active, move);

	for(int i = 0; i < active.size(); ++i) {
		Pokemon* p = active.at(i);
		if(p == NULL) continue;

		BattleTurn *turn = move.at(p->getMaster()->getGUID());
		if(turn == NULL) continue;

		if(turn->isMoveTurn()) {
			PokemonMove *pokemonMove = turn->getMove(p);
			if(pokemonMove != NULL) {
				pokemonMove->beginTurn(move, i, p);
			}
		}
	}

	for(int i = 0; i < active.size(); ++i) {
		BattleTurn *turn = move.at(i);
		if(turn != NULL) {
			executeTurn(turn, turn->getOwner(), turn->getTarget());
		}
	}

	// Refresh active pokemon vector in case a trainer switched
	active = getActivePokemon();

	tickStatuses(active);

	bool request = true;
	pokemonList_t::iterator it = active.begin();
	for(; it != active.end(); ++it) 
	{
		Pokemon *p = (*it);
		p->synchroniseStatuses();

		if(!p->isFainted()) {
			continue;;
		}

		requestPokemonReplacement(p->getMaster());
		request = false;
	}

	// Sync FieldEffects
	synchroniseFieldEffects();

	showMessage("---------");

	if(request) {
		requestsMoves();
	}
}

void BattleField::tickStatuses( pokemonList_t active )
{
	sortBySpeed(&active);
	pokemonList_t::iterator it;

	for(it = active.begin(); it != active.end(); ++it) {
		Pokemon* pokemon = (*it);
		pokemon->beginStatusTicks();
	}

	const int tiers = StatusEffect::getTierCount();
	for(int i = 0; i < tiers; ++i) 
	{
		for(it = active.begin(); it != active.end(); ++it) 
		{
			Pokemon* poke = (*it);
			if(poke->isFainted()) continue;

			statusEffectList_t list = poke->getStatusesByTier(i);
			for(; !list.empty(); list.pop_back()) {
				StatusEffect *eff = list.back();
				eff->tick(poke);
			}
		}		
	}
}

int BattleField::getAliveCount( Creature *trainer )
{
	return trainer->getPokemonParty()->getAliveCount();
}

void BattleField::sendMessage( BattleMessage* message )
{
	//TODO: BattleField::sendMessage
}

MoveListEntry* BattleField::getStruggle()
{
	return MoveList::getDefaultData()->getMove("Struggle");
}