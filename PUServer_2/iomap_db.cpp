#include "iomap_db.h"

#include "database.h"
#include "logger.h"
#include "map.h"
#include "warp.h"

bool IOMap_DB::loadMap( Map* map )
{
	Database* db = Database::getFactory();
	if(!db || !db->isConnected()) {
		setLastErrorMessage("Could not connect to database!");
		return false;
	}

	DBResult* res;
	DBQuery query;
	query << "SELECT t.`x`, t.`y`, t.`z`, t.`idlocation`, t.`idmap`, t.`movement`, t.`idteleport`,"
		  << " tl.`sprite`, tl.`layer`, tp.`x` AS `tp_x`, tp.`y` AS `tp_y`, tp.`z` AS `tp_z`"
		  << " FROM tile `t`"
		  << " INNER JOIN tile_layer `tl` ON tl.`idtile` = t.`idtile`"
		  << " LEFT JOIN teleport `tp` ON tp.`idteleport` = t.`idteleport`";

	if((res = db->storeQuery(query.str())) == NULL) {
		setLastErrorMessage("Error while executing tile query!");
		return false;
	}

	if(res->count() <= 0) {
		return true;
	}

	std::stringstream test;
	test << "Total loading size: " << res->count();
	Logger::consoleOutput(COUTPUT_MESSAGE, test.str());

	do 
	{
		int16_t x			= res->getDataInt("x");
		int16_t y			= res->getDataInt("y");
		int16_t z			= res->getDataInt("z");
		int32_t layer		= res->getDataInt("layer");
		int32_t sprite		= res->getDataInt("sprite");
		int32_t movem		= res->getDataInt("movement");
		int32_t tp_id		= res->getDataInt("idteleport");

		uint32_t location	= res->getDataInt("idlocation");
		_indexType index	= map->createIndex(x, y, z);

		TileLayer* layer_ptr	= new TileLayer(layer, sprite);
		Tile* tile_ptr			= NULL;

		if((tile_ptr = map->getTile(index)) == NULL) 
		{
			tile_ptr = new Tile(x, y, z);
			tile_ptr->setBlockingVolume(movem);

			if(tp_id > 0) 
			{
				int16_t tp_x	= res->getDataInt("tp_x");
				int16_t tp_y	= res->getDataInt("tp_y");
				int16_t tp_z	= res->getDataInt("tp_z");
				
				Warp* teleport = new Warp(Position(tp_x, tp_y, tp_z));
				tile_ptr->addTileEvent(teleport);
			}

			map->addTile(index, tile_ptr);
		}
	
		if(tile_ptr->getTown() == NULL && location != 0) {
			tile_ptr->setTown(location);
		}
		
		tile_ptr->addLayer(layer, layer_ptr);
	} while (res->next());

	return true;
}