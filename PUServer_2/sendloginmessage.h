#ifndef INC_SENDLOGINMESSAGE_H_
#define INC_SENDLOGINMESSAGE_H_

#include "definitions.h"
#include "netmessage.h"

class SendLoginMessage : public NetMessage
{
	int m_status;

public:
	SendLoginMessage() : NetMessage(HEADER_LOGIN) {};
	~SendLoginMessage() {};

	void addStatus(int status) { m_status = status; };

	virtual void completePacket(Packet& packet)
	{
		packet.AddByte(m_status);
	};

};
#endif