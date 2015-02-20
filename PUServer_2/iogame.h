#ifndef _INC_IOGAME_H_
#define _INC_IOGAME_H_

#include "definitions.h"

#include "game.h"

extern Game g_game;

class Map;

class IOGame
{
public:
	IOGame() {};
	~IOGame() {};

	bool loadItems();

	bool loadTowns();

	//bool loadHouses();
	
	bool loadQuests();
	
	bool loadSpawns();

	void setMap(Map* map) {	g_game.m_map = map; };
};

#endif