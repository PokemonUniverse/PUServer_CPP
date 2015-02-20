#ifndef __TILEHOUSE_H_
#define __TILEHOUSE_H_

#include "tile.h"

class House;

class HouseTile : public Tile
{
public:
	HouseTile(int x, int y, int z, House* _house);
	~HouseTile();

protected:
private:
};
#endif