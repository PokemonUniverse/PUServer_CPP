#ifndef __MAP_H_
#define __MAP_H_

#include <iostream>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "tile.h"

typedef int64_t _indexType;
typedef boost::unordered_map<_indexType, Tile*> TilesList;

class Map
{
public:
	Map() : isLoaded(false) {};
	~Map();

	bool load();

	TilesList getTilesList() { return mapTiles; };

	void addTile(_indexType index, Tile* tile) {
		TilesList::iterator it = mapTiles.find(index);
		if(it == mapTiles.end()) {
			mapTiles.insert(std::make_pair<_indexType, Tile*>(index, tile));
		}
	};

	Tile *getTile(_indexType index) {
		TilesList::iterator it = mapTiles.find(index);
		if(it == mapTiles.end()) {
			return NULL;
		}

		return it->second;
	};
	
	Tile *getTile(int16_t _x, int16_t _y, int16_t _z) { return getTile(createIndex(_x, _y, _z)); };

	static _indexType createIndex(int16_t _x, int16_t _y, int16_t _z = 0) {
		return (_z ^ (_x << 4) ^ (_y << 20));
	};

	static _indexType createIndex(const Position &p) {
		return createIndex(p.x, p.y, p.z);
	}

private:
	TilesList mapTiles;
	bool isLoaded;

	void loadSolution(std::string file);
	void loadMap(std::string file);
};

#endif