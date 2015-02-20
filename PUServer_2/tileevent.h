#ifndef INC_TILEEVENT_H_
#define INC_TILEEVENT_H_

#include "definitions.h"
#include "thing.h"

class Creature;

enum TileEventType_t
{
	EVENT_NONE = 0,
	EVENT_WARP = 1,
};

class TileEvent
{
public:
	TileEvent() {};
	virtual ~TileEvent() {};

	virtual TileEventType_t getEventType() { return EVENT_NONE; };

	virtual ReturnValue onCreatureEnter(Creature* creature) = 0;
	virtual ReturnValue onCreatureLeave(Creature* creature) = 0;
};
#endif