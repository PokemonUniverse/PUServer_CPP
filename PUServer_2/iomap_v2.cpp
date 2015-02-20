#include "iomap_v2.h"

#include "configmanager.h"
#include "map.h"
#include "tile.h"
#include "logger.h"

#include <boost/format.hpp>
#include <libxml/parser.h>

extern ConfigManager g_config;

bool IOMap_V2::loadMap( Map* map )
{
	std::string xmlWorldMapFileName = boost::str(boost::format("data/%s") % g_config.pString("SOLUTION"));
	xmlDocPtr doc = xmlParseFile(xmlWorldMapFileName.c_str());

	if(!doc) {
		Logger::consoleOutput(COUTPUT_ERROR, "Error parsing solution file");

		xmlFreeDoc(doc);
		return false;
	}

	xmlNodePtr root = xmlDocGetRootElement(doc);
	std::string rootStr = (char*)root->name;
	if(rootStr == "solution")
	{
		xmlNodePtr search = root->children;
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
							std::string mapNameStr = boost::str(boost::format("data/maps/%s") % mapNameChar);
							if(!readMap(map, mapNameStr)) {
								Logger::consoleOutput(COUTPUT_ERROR, getLastErrorMessage());
								return false;
							}
						}
					}

					maps = maps->next;
				}
			}

			search = search->next;
		}
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();

	return true;
}

bool IOMap_V2::readMap( Map* map, const std::string& identifier )
{
	xmlDocPtr doc = xmlParseFile(identifier.c_str());
	int isInsideMap = 0;
	std::string tmp;

	if(!doc)
	{
		setLastErrorMessage("Could not parse map: " + identifier);

		xmlFreeDoc(doc);
		return false;
	}

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
					uint32_t TileID;

					std::string tileStr = (char*)townTiles->name;

					if(tileStr == "tile")
					{
						x = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "x"));
						y = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "y"));

						Tile *newTile = map->getTile(x, y, isInsideMap);

						if(!newTile) { // Create new tile
							newTile = new Tile(x, y, isInsideMap);
							newTile->setTown(TownID);
						}
						else if(newTile->getTown() == NULL) {
							newTile->setTown(TownID);
						}

						if(xmlHasProp(townTiles, (const xmlChar *) "s") != NULL)
						{
							int sectionID = atoi((const char*)xmlGetProp(townTiles, (const xmlChar *) "s"));
							newTile->setSection(sectionID);
						}

						newTile->setBlockingVolume(TILEBLOCK_WALK);

						// Loop tile objects
						tileObjects = townTiles->children;
						while(tileObjects)
						{
							int movementID = 0, eventID = 0;
							std::string eventData;

							std::string infoStr = (char*)tileObjects->name;
							if(infoStr == "object")
							{
								TileLayer *newObject;
								z = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "z"));

								TileID = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "t"));
								movementID = atoi((const char*)xmlGetProp(tileObjects, (const xmlChar *) "m"));


								newObject = new TileLayer(z, TileID);

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
								newTile->addLayer(z, newObject);
							}

							// Next object
							tileObjects = tileObjects->next;
						}

						// Add tile
						uint32_t index = map->createIndex(x,y,z);
						map->addTile(index, newTile);
					}

					townTiles = townTiles->next;
				}
			}

			towns = towns->next;
		}
	}
	return true;
}