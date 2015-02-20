#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "map.h"

#include "configmanager.h"
#include "tools.h"
#include "towns.h"
#include "logger.h"

#include "iomap.h"
#include "iomap_v2.h"
#include "iomap_db.h"

extern ConfigManager g_config;

Map::~Map()
{
	foreach(TilesList::value_type it, mapTiles)
	{
		delete it.second;
	}

	mapTiles.clear();
}

bool Map::load()
{
	std::string mapType = g_config.pString("MAPTYPE");
	IOMap* loader = NULL;
	if(mapType == "xml") {
		loader = new IOMap_V2();
	}
	else if(mapType == "database" || mapType == "db") {
		loader = new IOMap_DB();
	}
	else {
		Logger::consoleOutput(COUTPUT_ERROR, "Could not determine map type.");
		return false;
	}

	if(loader == NULL) { return false; }

	if(loader->loadMap(this)) {
		Logger::consoleOutput(COUTPUT_MESSAGE, "Optimizing worldmap hashtable...");
		mapTiles.rehash(mapTiles.size() / mapTiles.max_load_factor() + 1);
	}
	else {
		return false;
	}

	return true;
}