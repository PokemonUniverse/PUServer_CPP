#ifndef INC_SENDPLAYERDATA_H_
#define INC_SENDPLAYERDATA_H_

#include "definitions.h"
#include "netmessage.h"

#include "outfit.h"
#include "position.h"

class SendPlayerData : public NetMessage
{
	uint64_t m_GUID;

	uint32_t m_pos_x, m_pos_y;
	uint32_t m_direction;
	uint32_t m_money;

	std::string m_name;

	Outfit* m_outfit;
public:
	SendPlayerData() : NetMessage(HEADER_PLAYERDATA) {};
	~SendPlayerData() {};

	void addGUID(uint64_t guid) { m_GUID = guid; };
	void addPosition(Position pos) {
		m_pos_x = pos.x;
		m_pos_y = pos.y;
	};
	void addDirection(uint32_t direction) { m_direction = direction; };
	void addMoney(uint32_t money) { m_money = money; };
	void addName(std::string name) { m_name = name; };
	void addOutfit(Outfit* outfit) { m_outfit = outfit; };

	virtual void completePacket(Packet& packet)
	{
		packet.AddLong(m_GUID);
		packet.AddString(m_name);
		packet.AddInt(m_pos_x);
		packet.AddInt(m_pos_y);
		packet.AddInt(m_direction);
		packet.AddLong(m_money);

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
	};
};

#endif