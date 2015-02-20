#ifndef INC_CREATURETURNMESSAGE_H_
#define INC_CREATURETURNMESSAGE_H_

#include "definitions.h"

#include "netmessage.h"
#include "packet.h"

class CreatureTurnMessage : public NetMessage
{
	Creature* m_creature;
	Direction m_dir;

public:
	//************************************
	// Method:    CreatureTurnMessage
	// FullName:  CreatureTurnMessage::CreatureTurnMessage
	// Access:    public 
	// Qualifier: Server -> Player
	// Parameter: Creature * creature
	//************************************
	CreatureTurnMessage(Creature* creature) : NetMessage(HEADER_SENDTURN) {
		m_creature = creature;
	};
	
	//************************************
	// Method:    CreatureTurnMessage
	// FullName:  CreatureTurnMessage::CreatureTurnMessage
	// Access:    public 
	// Qualifier: Player -> Server
	// Parameter: packet_ptr packet
	// Parameter: Creature * creature
	//************************************
	CreatureTurnMessage(packet_ptr packet, Player* creature) : NetMessage(HEADER_TURN)
	{
		m_dir = (Direction)packet->ReadInt();
		m_creature = creature;
	};

	void addDirection(Direction dir) { m_dir = dir; };

	void executeReceivedMessage() {
		bool ret = g_game.onPlayerTurn(m_creature->getPlayer(), m_dir);
	};

	virtual void completePacket(Packet& packet)
	{
		packet.AddLong(m_creature->getGUID());
		packet.AddInt(m_dir);	
	};
};

#endif