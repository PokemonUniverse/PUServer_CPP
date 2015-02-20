#include "creature.h"

#include "player.h"
#include "npc.h"
#include "pokemon.h"

uint64_t Creature::count = 0;
GUIDList Creature::UniqueIdList;

Creature::Creature()
{
	count++;

	if(count == ULLONG_MAX)
		count = 0;

	for(;;)
	{
		GUIDList::iterator find = std::find(UniqueIdList.begin(), UniqueIdList.end(), count);
		if(find != UniqueIdList.end()) {
			count++;
		}
		else {
			break;
		}
	}

	UniqueIdList.push_back(count);
	this->GUID = count;
	this->setDirection(DIR_SOUTH);
	this->lastStep = PUSYS_TIME();
	this->isRemoved = false;
	this->attackCharging = 0;
	this->level = 1;
	this->setMovement(VOLUME_WALK);

	setBattle(false);
	setBattlePointer(NULL);
}

Creature::~Creature()
{
	isRemoved = true;
	GUIDList::iterator find = std::find(UniqueIdList.begin(), UniqueIdList.end(), this->GUID);
	if(find != UniqueIdList.end()) {
		UniqueIdList.erase(find);	
	}
}

bool Creature::canSee(const Creature *creature)
{
	if(!creature) return false;
	return canSee(creature->getPosition());
}

bool Creature::canSee(const Position &pos)
{
	const Position myPos = getPosition();
	if(myPos.z != pos.z) {
		return false;
	}
	if(!Position::areInRange<VIEWPORT_X_CENTER, VIEWPORT_Y_CENTER, 0>(myPos, pos)) {
		return false;
	}

	return true;
}

int64_t Creature::getTimeSinceLastMove() const
{
	if(lastStep){
		return PUSYS_TIME() - lastStep;
	}

	return 0x7FFFFFFFFFFFFFFFLL;
}

void Creature::onCreatureMove(const Creature* creature, Cylinder* fromCylinder, Cylinder* toCylinder)
{
	if(creature == this) {
		lastStep = PUSYS_TIME();
	}
}

bool Creature::knowsCreature(const Creature *creature)
{
	if(!visibleCreatures.empty())
	{
		CreatureList::iterator find = std::find(visibleCreatures.begin(), visibleCreatures.end(), creature);
		if(find != visibleCreatures.end())
		{
			return true;
		}
	}
	return false;
}

void Creature::removeVisibleCreature(const Creature *creature)
{
	CreatureList::iterator find = std::find(visibleCreatures.begin(), visibleCreatures.end(), creature);
	if(find != visibleCreatures.end()) {
		visibleCreatures.erase(find);
	}
}

void Creature::addVisibleCreature(const Creature *creature)
{
	CreatureList::iterator find = std::find(visibleCreatures.begin(), visibleCreatures.end(), creature);
	if(find == visibleCreatures.end()) {
		visibleCreatures.push_back(const_cast<Creature*>(creature));
	}
}

void Creature::removeFromBattle()
{
	if(battle)
	{
		battle->removePlayer(battle->getBattlePlayerByPlayer(getPlayer()));
	}
}

void Creature::setActivePokemon()
{
	activePokemonSlot = -1;
	for (PokemonList::iterator it = pokemonParty.begin(); it != pokemonParty.end(); ++it)
	{
		if((*it).second != NULL && (*it).second->getStatus() != -1 && (*it).second->getPokemonHP_Left() > 0 && (*it).second->hasUsableAttack())
		{
			activePokemonSlot = (*it).first;
			break;
		}
	}
}

void Creature::setActivePokemon(int slotID)
{
	if(pokemonParty[slotID] != NULL)
		activePokemonSlot = slotID;
}

bool Creature::checkNextPokemon()
{
	if(pokemonParty.size() <= 0) { 
		return false;
	}

	for (PokemonList::iterator it = pokemonParty.begin(); it != pokemonParty.end(); ++it)
	{
		if((*it).second != NULL && (*it).second->getPokemonHP_Left() > 0 && (*it).second->hasUsableAttack()) {
			return true;
		}
	}

	return false;
}

int Creature::getPokemonCountActive()
{
	if(pokemonParty.size() <= 0) { 
		return 0;
	}

	int count = 0;
	for (PokemonList::iterator it = pokemonParty.begin(); it != pokemonParty.end(); ++it)
	{
		if((*it).second != NULL && (*it).second->getPokemonHP_Left() > 0 && (*it).second->hasUsableAttack()) {
			count++;
		}
	}

	return count;
}

void Creature::resetBattleInfo()
{
	if(getBattleAttackCharge() == 0)
	{
		moveOption = MOVE_NONE;
		moveParameter = -1;
		moveValidated = 0;
		playerStatus = PS_WAITING;
	}
	else {
		playerStatus = PS_READY;
	}
}

void Creature::changePokemonSlot( int oldSlot, int newSlot )
{
	if(oldSlot > newSlot) 
	{
		Pokemon *savePokemon = getPokemon(oldSlot);
		int currentSlot = oldSlot - 1;
		while(currentSlot >= newSlot)
		{
			Pokemon *tmp = getPokemon(currentSlot);
			setPokemon(currentSlot+1, tmp);
			currentSlot--;
		}
		setPokemon(newSlot, savePokemon);
	}
	else if(oldSlot < newSlot) 
	{
		Pokemon *savePokemon = getPokemon(oldSlot);
		int currentSlot = oldSlot + 1;
		while(currentSlot <= newSlot)
		{
			Pokemon *tmp = getPokemon(currentSlot);
			setPokemon(currentSlot-1, tmp);
			currentSlot++;
		}
		setPokemon(newSlot, savePokemon);
	}
	else {
		return;
	}

	if(getPlayer()->getFollower() != pokemonParty[0]) {
		getPlayer()->setFollower(pokemonParty[0]);
	}

	setActivePokemon();
}