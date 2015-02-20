#ifndef __BATTLE_H_
#define __BATTLE_H_

#include <vector>

#include "creature.h"

class Battle
{
public:
	static BattleField* launcer(std::vector<Creature*> playerList);

private:
	Battle();
	~Battle();
};

#endif