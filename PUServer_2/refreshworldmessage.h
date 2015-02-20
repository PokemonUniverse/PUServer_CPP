#ifndef INC_REFRESHWORLDMESSAGE_H_
#define INC_REFRESHWORLDMESSAGE_H_

#include "definitions.h"
#include "netmessage.h"

#include "player.h"

class RefreshWorldMessage : public NetMessage
{
	Player* m_player;
public:
	RefreshWorldMessage(packet_ptr packet, Player* player) : NetMessage(HEADER_REFRESHWORLD), m_player(player) {};
	RefreshWorldMessage() : NetMessage(HEADER_REFRESHCOMPLETE) {};

	~RefreshWorldMessage() {};

	void executeReceivedMessage()
	{
		m_player->sendMapData(DIR_NULL);

		// Send confirmation
		RefreshWorldMessage message;
		m_player->sendMessage(message);
	};

	void completePacket(Packet& packet) {};
};
#endif