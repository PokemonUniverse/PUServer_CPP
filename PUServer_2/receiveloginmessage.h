#ifndef INC_RECEIVELOGINMESSAGE_H_
#define INC_RECEIVELOGINMESSAGE_H_

#include "definitions.h"
#include "netmessage.h"
#include "packet.h"

class ReceiveLoginMessage : public NetMessage
{
	std::string m_username;
	std::string m_password;
	uint16_t m_client_version;

public:
	ReceiveLoginMessage(packet_ptr packet) : NetMessage(packet->ReadByte())
	{
		m_username		 = packet->ReadString();
		m_password		 = packet->ReadString();
		m_client_version = packet->ReadInt();
	};

	~ReceiveLoginMessage() {};

	std::string getUsername() { return m_username; };
	std::string getPassword() { return m_password; };

	uint16_t getClientVersion() { return m_client_version; };

	void completePacket(Packet& packet) {};
};
#endif