#ifndef __PACKET_H_
#define __PACKET_H_

#define NETWORKMESSAGE_MAXSIZE 16384

class Packet
{
public:
	Packet();
	Packet(Packet *_packet);
	~Packet() { };

	int getHeader();
	void setHeader();

	void Reset();

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