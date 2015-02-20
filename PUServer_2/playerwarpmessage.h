#ifndef INC_PLAYERWARPMESSAGE_H_
#define INC_PLAYERWARPMESSAGE_H_

#include "definitions.h"
#include "netmessage.h"

#include "position.h"

class PlayerWarpMessage : public NetMessage
{
	Position m_position;

public:
	PlayerWarpMessage() : NetMessage(HEADER_TELEPORT) {};
	~PlayerWarpMessage() {};

	void addDestination(Position pos) { m_position = pos; };

	virtual void completePacket(Packet& packet)
	{
		packet.AddInt(m_position.x);
		packet.AddInt(m_position.y);
	};

};
#endif