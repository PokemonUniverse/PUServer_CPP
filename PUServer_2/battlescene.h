#ifndef __BATTLESCENE_H_
#define __BATTLESCENE_H_

#include <queue>

#include "battleevents.h"

class BattleScene
{
public:
	BattleScene() {};
	~BattleScene() {};

	void addEvent(BattleEvent* _event) {
		sceneQueue.push(_event);
	};

	BattleEvent getFirst() { sceneQueue.pop(); };
	BattleEvent peekFirst() { sceneQueue.front(); };

	bool hasNext() { return sceneQueue.size() != 0; };
	int size() { return sceneQueue.size(); } ;

private:
	std::queue<BattleEvent*> sceneQueue;
};
#endif