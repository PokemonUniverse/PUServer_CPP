#ifndef INC_BATTLEMESSAGE_H_
#define INC_BATTLEMESSAGE_H_

#include "netmessage.h"

class BattleMessage : public NetMessage
{
public:
	BattleMessage() {};
	virtual ~BattleMessage() {};
};
#endif