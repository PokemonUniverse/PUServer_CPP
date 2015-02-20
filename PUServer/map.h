#ifndef __map_h_
#define __map_h_

#include <iostream>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <algorithm>
#include <string>
#include <map>
#include <boost/unordered_map.hpp>

#include "global.h"
#include "configmanager.h"
#include "tile.h"
#include "logwriter.h"

extern ConfigManager g_config;
extern LogWriter g_log;

class Tile;

typedef boost::unordered_map<int, Tile*> MapList;

class Map
{
public:
	Map() {};
	~Map();

	bool load();
	bool reload();

	void loadSolution(std::string fileName);
	void loadMap(std::string fileName);

	void addTile(Tile* tile);
	void addTile(Tile* tile, bool isInsideTile = false);

	mapIndex_t createPosIndex(const int x, const int y);
	Tile *getTile(int x, int y);
	Tile *getTile(int x, int y, int z);

private:
	MapList tiles;
	MapList tilesInside;
};

#endif