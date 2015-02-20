#ifndef INC_MISTEFFECT_H_
#define INC_MISTEFFECT_H_

#include "definitions.h"
#include "partyeffect.h"

class MistEffect : public PartyEffect
{
public:
	MistEffect() : PartyEffect(" was shrouded in Mist!", "'s mist wore off.") {};

	std::string getName() { return "Mist"; };
};
#endif