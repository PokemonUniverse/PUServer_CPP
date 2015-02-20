#ifndef INC_WARP_H_
#define INC_WARP_H_

#include "definitions.h"
#include "tileevent.h"

#include "position.h"

class Warp : public TileEvent
{
	Position m_destination;

public:
	Warp(Position destination) {
		m_destination = destination;
	};
	~Warp() {};

	const TileEventType_t getEventType() const { return EVENT_WARP; };

	Position getDestination() { m_destination; };

	ReturnValue onCreatureEnter(Creature* creature);
	ReturnValue onCreatureLeave(Creature* creature) { return RET_NOERROR; };
};

#endif