#include <iostream>
#include "packet.h"

Packet::Packet()
{
	Reset();
}

Packet::Packet(Packet *_packet)
{
	Reset();

	memcpy(m_MsgBuf, _packet->getBuffer(), _packet->getMessageSize()+2);
	m_MsgSize = _packet->getMessageSize();
	m_ReadPos = _packet->getReadPos();
}

Packet::~Packet()
{

}

void Packet::Reset()
{
	m_MsgSize = 0;
	m_ReadPos = 2;  
}

int Packet::getHeader()
{
	int size = ((m_MsgBuf[0]) | (m_MsgBuf[1] << 8));
	m_MsgSize = size;
	return size;
}

void Packet::setHeader()
{
	m_MsgBuf[0] = (unsigned char)(m_MsgSize);
	m_MsgBuf[1] = (unsigned char)(m_MsgSize >> 8);

	m_MsgSize += 2;
}

unsigned char Packet::ReadByte()
{
	return m_MsgBuf[m_ReadPos++];
}

unsigned short Packet::ReadInt()
{
	unsigned short v = ((m_MsgBuf[m_ReadPos]) | (m_MsgBuf[m_ReadPos+1] << 8));
	m_ReadPos += 2;
	return v;
}

unsigned long Packet::ReadLong()
{
	unsigned long v = ((m_MsgBuf[m_ReadPos] ) | (m_MsgBuf[m_ReadPos+1] <<  8) |
					  (m_MsgBuf[m_ReadPos+2] << 16) | (m_MsgBuf[m_ReadPos+3] << 24));
	m_ReadPos += 4;
	return v;
}

std::string Packet::ReadString()
{
	int stringlen = ReadInt();
	if (stringlen >= (16384 - m_ReadPos))
		return std::string();

	char* v = (char*)(m_MsgBuf+m_ReadPos);
	m_ReadPos += stringlen;
	return std::string(v, stringlen);
}

void Packet::AddByte(unsigned char value)
{
	if(!canAdd(1))
		return;
	m_MsgBuf[m_ReadPos++] = value;
	m_MsgSize++;
}

void Packet::AddInt(unsigned int value)
{
	if(!canAdd(2))
		return;

	m_MsgBuf[m_ReadPos++] = (unsigned char)(value);
	m_MsgBuf[m_ReadPos++] = (unsigned char)(value >> 8);
	m_MsgSize += 2;
}

void Packet::AddLong(unsigned long value)
{
	if(!canAdd(4))
		return;
	m_MsgBuf[m_ReadPos++] = (unsigned char)(value);
	m_MsgBuf[m_ReadPos++] = (unsigned char)(value >>  8);
	m_MsgBuf[m_ReadPos++] = (unsigned char)(value >> 16);
	m_MsgBuf[m_ReadPos++] = (unsigned char)(value >> 24);
	m_MsgSize += 4;
}


void Packet::AddString(const std::string &value)
{
	AddString(value.c_str());
}

void Packet::AddString(const char* value)
{
	unsigned short stringlen = (unsigned short) strlen(value);
	if(!canAdd(stringlen+2))
		return;

	AddInt(stringlen);
	strcpy((char*)m_MsgBuf + m_ReadPos, value);
	m_ReadPos += stringlen;
	m_MsgSize += stringlen;
}

void Packet::Send(boost::asio::ip::tcp::socket & socket, boost::system::error_code & error)
{
	if(!socket.is_open()) {
		return;
	}

	setHeader();
	try 
	{
		boost::asio::write(socket, 
			boost::asio::buffer(getBuffer(), getMessageSize()), 
			boost::asio::transfer_at_least(getMessageSize()), 
			error);

		if(error) {
			//std::cout << "Error sending packet[1] Code: " << error << " - " << error.message() << std::endl;
			return;
		}
	}
	catch(...) {
		std::cout << "Error sending packet[2]" << std::endl;
	}

	return;
}