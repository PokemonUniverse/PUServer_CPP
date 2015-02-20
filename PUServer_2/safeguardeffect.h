#ifndef INC_SAFEGUARDEFFECT_H_
#define INC_SAFEGUARDEFFECT_H_

#include "definitions.h"
#include "partyeffect.h"

#include "burneffect.h"
#include "freezeeffect.h"
#include "paralysiseffect.h"
#include "poisoneffect.h"
#include "sleepeffect.h"
#include "confuseeffect.h"

class SafeguardEffect : public PartyEffect
{
public:
	SafeguardEffect() : PartyEffect(" is protected by a veil!", "'s Safeguard wore off") {};
	std::string getName() { return "Safeguard"; };

	bool allowsStatus(StatusEffect* eff, Pokemon* source, Pokemon* target) {
		if(!isActive(target->getMaster()->getBattleTeamIdx())) return true;
		if(source == target) return true;

		return !(isAssignableFrom<BurnEffect*>(eff) || isAssignableFrom<FreezeEffect*>(eff) || isAssignableFrom<ParalysisEffect*>(eff) ||
				 isAssignableFrom<PoisonEffect*>(eff) || isAssignableFrom<SleepEffect*>(eff) || isAssignableFrom<ConfuseEffect*>(eff));
	}
};
#endif