#ifndef INC_CREATUREWALKMESSAGE_H_
#define INC_CREATUREWALKMESSAGE_H_

#include "definitions.h"

#include "netmessage.h"
#include "packet.h"

#include "game.h"
#include "player.h"

extern Game g_game;

class CreatureWalkMessage : public NetMessage
{
	Creature* m_creature;
	Direction m_dir;
	bool m_sendMapUpdate;
	
	Position m_fromPosition, m_toPosition;

public:
	//************************************
	// Method:    CreatureWalkMessage
	// FullName:  CreatureWalkMessage::CreatureWalkMessage
	// Access:    public 
	// Qualifier: Server -> Player
	// Parameter: Creature * creature
	//************************************
	CreatureWalkMessage(Creature* creature) : NetMessage(HEADER_WALK) {
		m_creature = creature;
	};
	
	//************************************
	// Method:    CreatureWalkMessage
	// FullName:  CreatureWalkMessage::CreatureWalkMessage
	// Access:    public 
	// Qualifier: Player -> Server
	// Parameter: packet_ptr packet
	// Parameter: Creature * creature
	//************************************
	CreatureWalkMessage(packet_ptr packet, Player* creature) : NetMessage(HEADER_WALK)
	{
		m_dir = (Direction)packet->ReadInt();
		m_sendMapUpdate = (packet->ReadInt() == 1) ? true : false;
		m_creature = creature;
	};

	void addPositions(Position _from, Position _to) {
		m_fromPosition = _from;
		m_toPosition = _to;
	};

	void executeReceivedMessage() {
		bool ret = g_game.onPlayerMove(m_creature->getPlayer(), m_dir, m_sendMapUpdate);
	};

	virtual void completePacket(Packet& packet)
	{
		packet.AddLong(m_creature->getGUID());
		packet.AddInt(m_fromPosition.x);
		packet.AddInt(m_fromPosition.y);
		packet.AddInt(m_toPosition.x);
		packet.AddInt(m_toPosition.y);		
	};
};
#endif