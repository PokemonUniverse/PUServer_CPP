#ifndef INC_NETMESSAGE_H_
#define INC_NETMESSAGE_H_

#include <boost/asio.hpp>

#include "definitions.h"
#include "packet.h"
#include "logger.h"

typedef boost::shared_ptr<Packet> packet_ptr;

enum packetheaders_t
{
	HEADER_LOGIN			= 0x01,
	HEADER_LOGOUT			= 0x02,
	HEADER_REFRESHCOMPLETE	= 0x03,

	HEADER_PLAYERDATA		= 0xAA,

	HEADER_WALK				= 0xB1,
	HEADER_TURN				= 0xB2,
	HEADER_TELEPORT			= 0xB3,
	HEADER_SENDTURN			= 0xB4,

	HEADER_SENDTILES		= 0xC1,
	HEADER_ADDCREATURE		= 0xC2,
	HEADER_REMOVECREATURE	= 0xC3,
	HEADER_REFRESHWORLD		= 0xC4,
};

class NetMessage
{
public:
	NetMessage(uint8_t header) { m_header = header; };

	virtual ~NetMessage() {};

	virtual uint8_t getHeader() { return m_header; };

	virtual void executeReceivedMessage() {};

	void send(boost::asio::ip::tcp::socket& socket)
	{
		// Make the packet
		Packet packet;
		packet.AddByte(m_header);
		completePacket(packet);

		// Send the packet
		if(!socket.is_open()) {
			return;
		}
		boost::system::error_code error;

		packet.setHeader();
		try	{
			boost::asio::write(socket, boost::asio::buffer(packet.getBuffer(), packet.getMessageSize()), boost::asio::transfer_at_least(packet.getMessageSize()), error);
		}
		catch( std::exception e ) {
			Logger::consoleOutput(COUTPUT_ERROR, "Client send error | " + std::string(e.what()), false);
		}
#ifdef __IS_DEBUG__
		if(error) {
			Logger::consoleOutput(COUTPUT_DEBUG, error.message(), false);
		}
#endif
	};

	virtual void completePacket(Packet& packet) = 0;

protected:
	uint8_t m_header;

	void setHeader(uint8_t header) { m_header = header; };
};
#endif