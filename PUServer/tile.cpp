#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "game.h"
#include "tile.h"
#include "creature.h"
#include "player.h"
#include "pokecode.h"

extern Game g_game;

////////////////
// TileObject Class    
////////////////
TileObject::TileObject(int _z, int _tile)
{
	z = _z;
	tileID = _tile;
}

TileObject::TileObject(int _z, int _tile, int _movement)
{
	z = _z;
	tileID = _tile;
	movement = _movement;
}

TileObject::~TileObject()
{
}

////////////////
// Tile Class
///////////////
Tile Tile::null_tile(0xFFFF, 0xFFFF);

Tile::Tile(int _x, int _y)
{
	pos.x = _x;
	pos.y = _y;

	objectList[0] = NULL;
	objectList[1] = NULL;
	objectList[2] = NULL;
}

Tile::Tile(int _x, int _y, int _z)
{
	pos.x = _x;
	pos.y = _y;
	pos.z = _z;

	objectList[0] = NULL;
	objectList[1] = NULL;
	objectList[2] = NULL;
}

Tile::~Tile()
{
}

void Tile::addObject(TileObject *tileObject)
{
	objectList[tileObject->getZ()] = tileObject;
}

void Tile::removeObject(TileObject *tileObject)
{
	objectList[tileObject->getZ()] = NULL;
	delete tileObject;
}

void Tile::addCreature(Creature *creature)
{
	if(!this || !creature || creature->isRemoved)
		return;

	std::vector<Creature*>::iterator it = std::find(players.begin(), players.end(), creature);
	if(it == players.end())
	{
		players.push_back(creature);
	}
}

void Tile::removeCreature(Creature *creature)
{
	if(!this || !creature || creature->isRemoved)
		return;

	std::vector<Creature*>::iterator it = std::find(players.begin(), players.end(), creature);
	if(it != players.end())
	{
		players.erase(it);
	}
}

Creature *Tile::getFirstCreature()
{
	if(!this)
		return NULL;

	std::vector<Creature*>::iterator it = players.begin();
	std::vector<Creature*>::iterator ite = players.end(); 
	for( ; it != ite; ++it)
	{
		Creature *tmp = (*it);
		if(!tmp)
			continue;

		return tmp;
	}

	return NULL;
}

bool Tile::moveCreature(Creature *creature, Direction dir, Cylinder *toCylinder)
{
	removeCreature(creature);
	toCylinder->getTile()->addCreature(creature);

	creature->setPosition(toCylinder->getPosition());
	creature->setDirection(dir);

	if(creature->getPlayer() != NULL) 
	{
		// Update player's town info
		Town *town = creature->getPlayer()->getLastTown();
		if(toCylinder->getTile()->getTown() > 0 || (town && town->getTownID() != toCylinder->getTile()->getTown())) {
			creature->getPlayer()->setLastTown(toCylinder->getTile()->getTown());
		}

		// Check for warp event
		Tile *toTile = toCylinder->getTile();
		if(toTile->getObject(1) != NULL && toTile->getObject(1)->getEventID() == -1) 
		{
			string *warpEvent = new string[4];
			PokeCode::Compile(warpEvent, (string)toCylinder->getTile()->getObject(1)->getEventData());

			int newX = atoi(warpEvent[1].c_str());
			int newY = atoi(warpEvent[2].c_str());
			int newZ = atoi(warpEvent[3].c_str());

			delete [] warpEvent;

			Tile *newCylinder = g_game.getTile(newX, newY, newZ);
			if(newCylinder != NULL) {
				return true;
			}
		}
	}

	return false;
}

ReturnValue Tile::checkMovement(Creature *creature, Direction dir)
{
	int creatureMovement = creature->getMovement();

	int tileMovement = 1;
	for(int i = MAP_LAYERS; i >= 0; --i)
	{
		if(getObject(i) != NULL) {
			tileMovement = getObject(i)->getMovement();
			break;
		}
	}

	if(tileMovement != VOLUME_WALK)
	{
		if(tileMovement == VOLUME_BLOCK ||
			(tileMovement == VOLUME_SURF && creatureMovement != VOLUME_SURF) ||
			(tileMovement == VOLUME_BLOCKTOP && dir == DIR_SOUTH) ||
			(tileMovement == VOLUME_BLOCKBOTTOM && dir == DIR_NORTH) ||
			(tileMovement == VOLUME_BLOCKLEFT && dir == DIR_EAST) ||
			(tileMovement == VOLUME_BLOCKRIGHT && dir == DIR_WEST) ||
			(tileMovement == VOLUME_BLOCKCORNER_TL && (dir == DIR_EAST || dir == DIR_SOUTH)) ||
			(tileMovement == VOLUME_BLOCKCORNER_TR && (dir == DIR_WEST || dir == DIR_SOUTH)) ||
			(tileMovement == VOLUME_BLOCKCORNER_BL && (dir == DIR_EAST || dir == DIR_NORTH)) ||
			(tileMovement == VOLUME_BLOCKCORNER_BR && (dir == DIR_WEST || dir == DIR_NORTH))
			)
		{
			return RET_NOTPOSSIBLE;
		}
	}

	// Check if layer 2 has warp event on it
	// Npc's can't walk on warps
	TileObject *object = getObject(1);
	if(creature->getNpc() && object && object->getEventID() == -1) {
		return RET_NOTPOSSIBLE;
	}

	return RET_NOERROR;
}