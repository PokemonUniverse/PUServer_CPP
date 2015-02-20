#ifndef __HOUSE_H_
#define __HOUSE_H_

#include <boost/unordered_map.hpp>

#include "item.h"

class Door : public Item
{
public:
	Door(uint16_t _type);
	virtual ~Door();
};

class House
{
public:
	House(uint32_t _id);
	~House();

private:
	typedef std::list<Door*> HouseDoorList;
	HouseDoorList houseDoorList;
};

class Houses
{
	Houses();

public:
	static Houses* instance(){
		static Houses instance;
		return &instance;
	}

	~Houses();

	bool load();

private:
	typedef boost::unordered_map<uint32_t, House*> HouseMap;
	HouseMap houeMap;
};


#endif