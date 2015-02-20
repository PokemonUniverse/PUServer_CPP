#ifndef INC_RATIOREFRESHMESSAGE_H_
#define INC_RATIOREFRESHMESSAGE_H_

#include "definitions.h"
#include "battlemessage.h"

class RatioRefreshMessage : public BattleMessage
{
	int m_fid;
	doubleArray_t m_ratio;

public:
	RatioRefreshMessage(int fid, doubleArray_t ratio) : BattleMessage() {};
};
#endif