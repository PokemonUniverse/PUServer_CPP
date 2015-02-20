#include "game.h"

#include "creature.h"
#include "player.h"

#include "logger.h"
#include "iogame.h"
#include "map.h"
#include "towns.h"
#include "tools.h"

// NetMessage classes
#include "playerWalkMessage.h"
#include "playerwarpmessage.h"
#include "creatureturnmessage.h"

Game::Game() {
	m_map = new Map();
}

Game::~Game() {
	
}

bool Game::loadGame()
{
	IOGame* loader = new IOGame();

	Logger::consoleOutput(COUTPUT_LOAD, "Game item data...");
	if(!loader->loadItems()) {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not load item data.");
		return false;
	}

	Logger::consoleOutput(COUTPUT_LOAD, "Location and encounter data...");
	if(!loader->loadTowns()) {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not load location data.");
		return false;
	}

	Logger::consoleOutput(COUTPUT_LOAD, "Map");
	if(!m_map->load()) {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not load map data.");
		return false;
	}

	if(!loader->loadQuests()) {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not load quest data.");
		return false;
	}

	if(!loader->loadSpawns()) {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not load spawn data.");
		return false;
	}

	return true;
}

bool Game::stopGame()
{
	return true;
}

bool Game::reloadGame()
{
	return true;
}

Player* Game::getPlayerByID( uint32_t idx )
{
	return NULL;
}

Creature* Game::getCreatureByGUID( uint32_t idx )
{
	return NULL;
}

Tile* Game::getTile( const Position position )
{
	return getTile(position.x, position.y, position.z);
}

Tile* Game::getTile( int32_t x, int32_t y, int32_t z )
{
	return m_map->getTile(x, y, z);
}

void Game::addCreature( Creature *creature )
{
	boost::upgrade_lock<boost::shared_mutex> upgradeLock(m_creatureMutex);
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(upgradeLock);

	creatureList_t creatures = m_creatureList;
	Creature* c = NULL;
	foreach(creatureList_t::value_type it, creatures)
	{
		if((c = it.second) == NULL || c->isRemoved()) continue;
		c->onCreatureAppear(creature, true);
	}

	uint32_t guid = creature->getGUID();
	creatureList_t::iterator find = m_creatureList.find(guid);
	if(find == m_creatureList.end()) {
		m_creatureList.insert(std::make_pair<uint32_t, Creature*>(guid, creature));
	}
}

void Game::removeCreature( Creature *creature, bool needLock /* = true */ )
{
	uint32_t guid = creature->getGUID();
	
	if(needLock) { // Get write lock for deletion
		boost::upgrade_lock<boost::shared_mutex> upgradeLock(m_creatureMutex);
		boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(upgradeLock);
	} 

	creatureList_t::iterator find = m_creatureList.find(guid);
	if(find == m_creatureList.end()) return;
	m_creatureList.erase(find);

	creatureList_t creatures = m_creatureList;
	Creature* c = NULL;
	foreach(creatureList_t::value_type it, creatures)
	{
		if((c = it.second) == NULL || c->isRemoved()) continue;
		c->onCreatureDisappear(creature, true);
	}
}

bool Game::onPlayerMove( Player* player, Direction direction, bool sendMapUpdate /*= true*/ )
{
	if(player == NULL) return false;
	
	Position currentPosition = player->getPosition();
	if(!player->canMove() || player->isInBattle()) {
		CreatureWalkMessage message(player);
		message.addPositions(currentPosition, currentPosition);
		player->sendMessage(message);

		return false;
	}

	ReturnValue ret = internalCreatureMove(player->getCreature(), direction);
	if(ret == RET_NOTPOSSIBLE) {
		CreatureWalkMessage message(player);
		message.addPositions(currentPosition, currentPosition);
		player->sendMessage(message);

		return false;
	}

	if(sendMapUpdate || ret == RET_PLAYERISTELEPORTED) 
	{
		if(ret == RET_PLAYERISTELEPORTED) {
			PlayerWarpMessage warp;
			warp.addDestination(player->getPosition());
			player->sendMessage(warp);
		}

		player->sendMapData( ((ret == RET_PLAYERISTELEPORTED) ? DIR_NULL : direction) );
	}
	
	return true;
}

bool Game::onPlayerTurn( Player* player, Direction direction )
{
	if(player->getDirection() == direction) return false;

	ReturnValue ret = internalCreatureTurn(player, direction);

	return (ret == RET_NOERROR) ? true : false;
}

ReturnValue Game::internalCreatureMove( Creature* creature, Direction dir )
{
	Position currentPosition = creature->getPosition();
	Tile* currentTile = getTile(currentPosition);
	Position destinationPosition = currentPosition;

	switch(dir) 
	{
		case DIR_NORTH:
			destinationPosition.y -= 1;
			break;
		case DIR_SOUTH:
			destinationPosition.y += 1;
			break;
		case DIR_WEST:
			destinationPosition.x -= 1;
			break;
		case DIR_EAST:
			destinationPosition.x += 1;
			break;
	}

	Tile* destinationTile = getTile(destinationPosition);
	if(destinationTile == NULL) {
		return RET_NOTPOSSIBLE;
	}

	// Check if we can move to the destination tile
	ReturnValue ret = destinationTile->checkMovement(creature, dir);
	if(ret == RET_NOTPOSSIBLE) {
		return ret;
	}

	// Send creature move to all other creatures
	boost::shared_lock<boost::shared_mutex> lock(m_creatureMutex);
	creatureList_t creatures = m_creatureList;
	Creature* c = NULL;
	foreach(creatureList_t::value_type it, creatures) // TODO: Might want to optimize this?
	{
		if((c = it.second) == NULL || c->isRemoved()) continue;
		c->onCreatureMove(creature, currentTile, destinationTile);
	}


	// Remove creature from current tile
	if(currentTile->removeCreature(creature) == RET_NOTPOSSIBLE) {
		return RET_NOTPOSSIBLE;
	}

	// Add creature to the destination tile
	ret = destinationTile->addCreature(creature);
	if(ret == RET_NOTPOSSIBLE) {
		return RET_NOTPOSSIBLE;
	}
	
	if(ret != RET_PLAYERISTELEPORTED) {
		creature->setTile(destinationTile); // Update the position of the creature
	}

	if(creature->getPlayer()) 
	{	
		//TODO: Check for wild encounters
	}

	return ret;
}

ReturnValue Game::internalCreatureTeleport( Creature* creature, Tile* from, Tile* to, bool internalOnly /* = false */ )
{
	if(internalOnly) 
	{
		// Remove creature from current tile
		if(from->removeCreature(creature) == RET_NOTPOSSIBLE) {
			return RET_NOTPOSSIBLE;
		}

		// Add creature to the destination tile
		ReturnValue ret = to->addCreature(creature);
		if(ret == RET_NOTPOSSIBLE) {
			return RET_NOTPOSSIBLE;
		}
	}

	// Update the position of the creature
	creature->setTile(to);

	boost::shared_lock<boost::shared_mutex> lock(m_creatureMutex);
	creatureList_t creatures = m_creatureList;
	Creature* c = NULL;
	foreach(creatureList_t::value_type it, creatures)
	{
		if((c = it.second) == NULL || c->isRemoved()) continue;
		c->onCreatureMove(creature, from, to, true);
	}

	return RET_PLAYERISTELEPORTED;
}

ReturnValue Game::internalCreatureTurn( Creature* creature, Direction dir )
{
	if(!creature || creature->getDirection() == dir) {
		return RET_NOERROR;
	}

	creature->setDirection(dir);

	CreatureList visibleCreatures = creature->getVisibleCreatures();
	foreach(Creature* c, visibleCreatures)
	{
		if(!c || c->getPlayer() == NULL || c->isRemoved()) continue;

		c->onCreatureTurn(creature, dir);
	}

	return RET_NOERROR;
}