#ifndef __TOWN_H__
#define __TOWN_H__

#include <string>
#include <list>
#include <map>

#include "position.h"
#include "definitions.h"

class Town
{
public:
	Town(uint32_t _townid)
	{
		townid = _townid;
		musicid = 0;
		pokecenter = Position(0,0,0);
	}

	~Town(){};

	uint32_t getTownID() { return townid; }
	uint32_t getMusicID() { return musicid; }
	Position getPokeCenter() { return pokecenter; }
	std::string getName() { return townName; }

	void setPokeCenter(Position _pokecenter) { pokecenter = _pokecenter; }
	void setName(std::string _townName) { townName = _townName; }
	void setMusicID(int _musicID) { musicid = _musicID; }

private:
	uint32_t townid;
	uint32_t musicid;
	std::string townName;
	Position pokecenter;
};

typedef std::map<uint32_t, Town*> TownMap;

class Towns
{
public:
	Towns()
	{
		Town *town = new Town(9999);
		town->setName("PokeCenter");
		town->setMusicID(31);
		addTown(9999, town);
	}
	
	static Towns& getInstance()
	{
		static Towns instance;
		return instance;
	}

	bool addTown(uint32_t _townid, Town* town)
	{
		TownMap::iterator it = townMap.find(_townid);

		if(it != townMap.end()){
			return false;
		}

		townMap[_townid] = town;
		return true;
	}

	Town* getTown(std::string& townname)
	{
		for(TownMap::iterator it = townMap.begin(); it != townMap.end(); ++it){
			if(strcasecmp(it->second->getName().c_str(), townname.c_str()) == 0){
				return it->second;
			}
		}

		return NULL;
	}

	Town* getTown(uint32_t _townid)
	{
		TownMap::iterator it = townMap.find(_townid);

		if(it != townMap.end()){
			return it->second;
		}

		return NULL;
	}

	TownMap::const_iterator getTownBegin() const{return townMap.begin();}
	TownMap::const_iterator getTownEnd() const{return townMap.end();}

private:
	TownMap townMap;
};


#endif