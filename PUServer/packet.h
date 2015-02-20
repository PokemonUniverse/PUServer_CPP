#ifndef __packet_h_
#define __packet_h_

#include <boost/asio.hpp>

#define NETWORKMESSAGE_MAXSIZE 16384

typedef boost::shared_ptr<boost::asio::ip::tcp::socket>socket_pointer;

class Packet
{
public:
	Packet();
	Packet(Packet *_packet);
	~Packet();

	int getHeader();
	void setHeader();

	void Reset();
	void Send(boost::asio::ip::tcp::socket & socket, boost::system::error_code& error);

	unsigned char  ReadByte();
	unsigned short ReadInt();
	unsigned long  ReadLong();
	std::string    ReadString();

	void Skip(int count);

	void AddByte(unsigned char  value);
	void AddInt (unsigned int value);
	void AddLong (unsigned long value);
	void AddString(const std::string &value);
	void AddString(const char* value);

	int getMessageLength(){
		return m_MsgSize-m_ReadPos;
	}

	int getMessageSize(){
		return m_MsgSize;
	}

	int getReadPos(){
		return m_ReadPos;
	}

	char* getBuffer() { 
		return (char*)&m_MsgBuf[0]; 
	}
	char* getDataBuffer() { 
		m_ReadPos = 2; return (char*)&m_MsgBuf[2]; 
	}

protected:
	inline bool canAdd(int size){
		return (size + m_ReadPos < NETWORKMESSAGE_MAXSIZE - 16);
	};

	unsigned long m_MsgSize;
	int m_ReadPos;

	unsigned char m_MsgBuf[NETWORKMESSAGE_MAXSIZE];
};

#endif