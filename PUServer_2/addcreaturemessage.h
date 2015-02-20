#ifndef INC_ADDCREATUREMESSAGE_H_
#define INC_ADDCREATUREMESSAGE_H_

#include "definitions.h"

#include "netmessage.h"
#include "packet.h"

#include "creature.h"
#include "outfit.h"

class AddCreatureMessage : public NetMessage
{
	Creature* m_creature;
	Outfit* m_outfit;

public:
	AddCreatureMessage() : NetMessage(HEADER_ADDCREATURE) {};
	~AddCreatureMessage() {};

	void addCreature(Creature* creature) { m_creature = creature; };
	void addOutfit(Outfit* outfit) { m_outfit = outfit; };

	void completePacket(Packet& packet)
	{
		packet.AddLong(m_creature->getGUID());
		packet.AddString(m_creature->getName());
		packet.AddInt(m_creature->getPosition().x);
		packet.AddInt(m_creature->getPosition().y);
		packet.AddInt(m_creature->getDirection());
		
		packet.AddByte(m_outfit->getStyle(OUTFIT_UPPER));
		packet.AddLong(m_outfit->getColour(OUTFIT_UPPER));
		packet.AddByte(m_outfit->getStyle(OUTFIT_NEK));
		packet.AddLong(m_outfit->getColour(OUTFIT_NEK));
		packet.AddByte(m_outfit->getStyle(OUTFIT_HEAD));
		packet.AddLong(m_outfit->getColour(OUTFIT_HEAD));
		packet.AddByte(m_outfit->getStyle(OUTFIT_FEET));
		packet.AddLong(m_outfit->getColour(OUTFIT_FEET));
		packet.AddByte(m_outfit->getStyle(OUTFIT_LOWER));
		packet.AddLong(m_outfit->getColour(OUTFIT_LOWER));
	}
};
#endif