#include "warp.h"

#include "creature.h"
#include "tile.h"
#include "game.h"

extern Game g_game;

ReturnValue Warp::onCreatureEnter( Creature* creature )
{
	Tile* currentTile = g_game.getTile(creature->getPosition());
	Tile* destinationTile = g_game.getTile(m_destination);

	if(currentTile == NULL || destinationTile == NULL) {
		return RET_NOTPOSSIBLE;
	}

	return g_game.internalCreatureTeleport(creature, currentTile, destinationTile);
}