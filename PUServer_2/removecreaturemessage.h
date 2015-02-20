#ifndef INC_REMOVECREATUREMESSAGE_H_
#define INC_REMOVECREATUREMESSAGE_H_

#include "definitions.h"

#include "netmessage.h"
#include "packet.h"

#include "creature.h"

class RemoveCreatureMessage : public NetMessage
{
	Creature* m_creature;

public:
	RemoveCreatureMessage(Creature* creature) : NetMessage(HEADER_REMOVECREATURE), m_creature(creature) {};
	~RemoveCreatureMessage() {};

	void completePacket(Packet& packet)
	{
		packet.AddLong(m_creature->getGUID());
	}
};

#endif