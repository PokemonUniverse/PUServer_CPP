#include "iogame.h"

#include "items.h"
#include "towns.h"
#include "quest.h"

bool IOGame::loadItems()
{
	return Items::instance()->load();
}

bool IOGame::loadTowns()
{
	return Towns::getInstance()->load();
}

bool IOGame::loadQuests()
{
	//return Quests::instance()->load();
	return true;
}

bool IOGame::loadSpawns()
{
	
	return true;
}