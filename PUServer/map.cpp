#include "map.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>

Map::~Map()
{
	BOOST_FOREACH(MapList::value_type it, tiles) {
		delete it.second;
	}
	tiles.clear();

	BOOST_FOREACH(MapList::value_type it, tilesInside) {
		delete it.second;
	}
	tilesInside.clear();
}

bool Map::load()
{
	std::string xmlWorldMapFileName;
	xmlWorldMapFileName = g_config.pString("xmlWorldMap");
	loadSolution(xmlWorldMapFileName);

	return true;
}

bool Map::reload()
{
	BOOST_FOREACH(MapList::value_type it, tiles) {
		delete it.second;
	}
	tiles.clear();

	BOOST_FOREACH(MapList::value_type it, tilesInside) {
		delete it.second;
	}
	tilesInside.clear();

	return load();
}

void Map::loadSolution(std::string fileName)
{
	xmlDocPtr doc;	
	std::string solutionDir = g_config.pString("dataFolder") + "/" + fileName;
	doc = xmlParseFile(solutionDir.c_str());

	if(doc)
	{
		xmlNodePtr root, search;
		root = xmlDocGetRootElement(doc);
		std::string rootstr = (char*)root->name; 

		if(rootstr == "solution")
		{
			search = root->children;
			while(search)
			{
				std::string searchStr = (char*)search->name;
				if(searchStr == "maps")
				{
					xmlNodePtr maps = search->children;
					while(maps)
					{
						std::string mapStr = (char*)maps->name;

						if(mapStr == "map")
						{
							if(xmlHasProp(maps, (const xmlChar *) "name") != NULL)
							{
								char* mapNameChar = (char*)xmlGetProp(maps, (const xmlChar *)"name");
								std::string mapNameStr = g_config.pString("dataFolder") + "/maps/" + mapNameChar;

								std::cout << "\t* " << mapNameChar << std::endl;

								loadMap(mapNameStr);
							}
						}
						maps = maps->next;
					}
				}
				search = search->next;
			}
		}

		xmlFreeDoc(doc);
	}

	doc = NULL;
}

void Map::loadMap(std::string fileName)
{
	xmlDocPtr doc;
	doc = xmlParseFile(fileName.c_str());
	int isInsideMap = 0;
	std::string tmp;

	if(doc)
	{
		xmlNodePtr root, towns, townTiles, tileObjects;
		root = xmlDocGetRootElement(doc);
		std::string rootstr = (char*)root->name; 

		if(rootstr == "worldmap")
		{			
			towns = root->children;
			while(towns)
			{
				std::string townStr = (char*)towns->name;
				if(townStr == "mapInfo")
				{
					int type = atoi((const char*)xmlGetProp(towns, (const xmlChar *) "type"));
					isInsideMap = type;
				}
				else if(townStr == "town")
				{
					int TownID = atoi((const char*)xmlGetProp(towns, (const xmlChar *) "townid"));
					char* TownName_Char = (char*)xmlGetProp(towns, (const xmlChar *) "name");
					std::string TownName = boost::str(boost::format("%s") % TownName_Char);
					int pokeCenter = 0;

					if(xmlHasProp(towns, (const xmlChar *) "pc") != NULL)
						pokeCenter = atoi((const char*)xmlGetProp(towns, (const xmlChar *) "pc"));

					townTiles = towns->children;
					while(townTiles)
					{
						int x, y, z;
						long TileID;

						std::string tileStr = (char*)townTiles->name;

						if(tileStr == "tile")
						{
							x = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "x"));
							y = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "y"));

							Tile *newTile = getTile(x, y, isInsideMap);

							if(!newTile) { // Create new tile
								newTile = new Tile(x, y, isInsideMap);
								newTile->setTown(TownID);
								newTile->setTownName(TownName);
								newTile->setPokeCenterID(pokeCenter);
							}

							if(xmlHasProp(townTiles, (const xmlChar *) "s") != NULL)
							{
								int sectionID = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "s"));
								newTile->setSection(sectionID);
							}

							// Loop tile objects
							tileObjects = townTiles->children;
							while(tileObjects)
							{
								int movementID = 0, eventID = 0;
								std::string eventData;

								std::string infoStr = (char*)tileObjects->name;
								if(infoStr == "object")
								{
									TileObject *newObject;
									z = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "z"));

									TileID = atol((const char*)xmlGetProp(tileObjects, (const xmlChar *) "t"));

									movementID = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "m"));

									newObject = new TileObject(z, TileID, movementID);

									// Check for event property
									if(xmlHasProp(tileObjects, (const xmlChar *) "e") != NULL)
									{
										eventID = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "e"));
										newObject->setEventID(eventID);

										if(xmlHasProp(tileObjects, (const xmlChar *) "ed") != NULL)
										{
											char* eventData = (char*)xmlGetProp(tileObjects, (const xmlChar *)"ed");
											newObject->setEventData(eventData);
										}
									}

									// Add tile object to new tile
									newTile->addObject(newObject);
								}

								// Next object
								tileObjects = tileObjects->next;
							}

							// Add tile
							addTile(newTile, (isInsideMap == 1));
						}

						townTiles = townTiles->next;
					}
				}

				towns = towns->next;
			}
		}

		xmlFreeDoc(doc); 
	}

	doc = NULL;
}

mapIndex_t Map::createPosIndex(const int x, const int y)
{
	//return std::pair<int,int>(x,y);
	int key = x;
	key ^= y << 16;	


	return key;
}

void Map::addTile(Tile *tile, bool isInsideTile)
{
	if(!tile)
		return;

	mapIndex_t posIndex = createPosIndex(tile->getX(),tile->getY());

	if(isInsideTile)
	{
		if(tilesInside.find(posIndex) == tilesInside.end())	{
			tilesInside.insert(std::make_pair(posIndex, tile));
		}
	}
	else
	{
		if(tiles.find(posIndex) == tiles.end()) {
			tiles.insert(std::make_pair(posIndex, tile));
		}
	}
}

Tile *Map::getTile(int x, int y)
{
	mapIndex_t posIndex = createPosIndex(x,y);
	MapList::iterator it = tiles.find(posIndex);

	if(it != tiles.end())
	{
		return it->second;
	}

	return NULL;
}

Tile *Map::getTile(int x, int y, int z)
{
	if(z == 1)
	{
		mapIndex_t posIndex = createPosIndex(x,y);
		MapList::iterator it = tilesInside.find(posIndex);

		if(it != tilesInside.end())
		{
			return it->second;
		}
	}
	else
	{
		return getTile(x, y);
	}

	return NULL;
}