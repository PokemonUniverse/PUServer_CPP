#include "tile.h"

#include "position.h"
#include "creature.h"
#include "tileevent.h"
#include "warp.h"
#include "towns.h"

Tile::Tile(int _x, int _y, int _z) {
	m_position = Position(_x, _y, _z);
	m_town = NULL;
	m_event = NULL;
	m_blockingvolume = TILEBLOCK_WALK;
}

Tile::Tile(Position pos) {
	Tile(pos.x, pos.y, pos.z);
}

ReturnValue Tile::checkMovement( Creature *creature, Direction dir )
{
	creaturemovement_t creatureMovement = creature->getMovement();
	if(m_blockingvolume != TILEBLOCK_WALK)
	{
		if(m_blockingvolume == TILEBLOCK_BLOCK ||
			(m_blockingvolume == TILEBLOCK_SURF && creatureMovement != MOVEMENT_SURF) ||
			(m_blockingvolume == TILEBLOCK_TOP && dir == DIR_SOUTH) ||
			(m_blockingvolume == TILEBLOCK_BOTTOM && dir == DIR_NORTH) ||
			(m_blockingvolume == TILEBLOCK_LEFT && dir == DIR_EAST) ||
			(m_blockingvolume == TILEBLOCK_RIGHT && dir == DIR_WEST) ||
			(m_blockingvolume == TILEBLOCK_TOPLEFT && (dir == DIR_EAST || dir == DIR_SOUTH)) ||
			(m_blockingvolume == TILEBLOCK_TOPRIGHT && (dir == DIR_WEST || dir == DIR_SOUTH)) ||
			(m_blockingvolume == TILEBLOCK_BOTTOMLEFT && (dir == DIR_EAST || dir == DIR_NORTH)) ||
			(m_blockingvolume == TILEBLOCK_BOTTOMRIGHT && (dir == DIR_WEST || dir == DIR_NORTH))
			)
		{
			return RET_NOTPOSSIBLE;
		}
	}

	if(creature->getNpc() != NULL && m_event->getEventType() == EVENT_WARP) {
		return RET_NOTPOSSIBLE;
	}

	return RET_NOERROR;
}

ReturnValue Tile::addCreature( Creature *creature )
{
	activeCreatures_t::iterator it = std::find(m_activeCreatures.begin(), m_activeCreatures.end(), creature);
	if(it == m_activeCreatures.end()) {
		m_activeCreatures.push_back(creature);
	}
	
	ReturnValue ret = RET_NOERROR;
	if(m_event) {
		ret = m_event->onCreatureEnter(creature);
	}

	return ret;
}

ReturnValue Tile::removeCreature( Creature *creature )
{
	if(m_event) {
		ReturnValue ret;
		if((ret = m_event->onCreatureLeave(creature)) == RET_NOTPOSSIBLE) {
			return ret;
		}
	}

	activeCreatures_t::iterator it = std::find(m_activeCreatures.begin(), m_activeCreatures.end(), creature);
	if(it != m_activeCreatures.end()) {
		m_activeCreatures.erase(it);
	}

	return RET_NOERROR;
}

void Tile::setTown( uint32_t _townID )
{
	m_town = Towns::getInstance()->getTown(_townID);
}