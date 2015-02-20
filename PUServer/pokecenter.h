#ifndef __pokecenter_h_
#define __pokecenter_h_

#include <iostream>
#include <sstream>

#include "database.h"
#include "player.h"

class Player;

class PokeCenter
{
public:
	PokeCenter();
	~PokeCenter();

	static void healActivePokemon(Player *player);

private:

};

#endif