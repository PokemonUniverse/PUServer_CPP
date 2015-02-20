#include "battlefield_pvp.h"

#include "intrinsicability.h"
#include "pokemon.h"
#include "statuseffect.h"

BattleField_PvP::~BattleField_PvP()
{
	BattleField::~BattleField();

	// rest
}

void BattleField_PvP::updateStatus( Pokemon* p, StatusEffect* eff )
{
	if(eff == NULL) return;

	int lock = eff->getLock();
	if(lock == 0 || lock == SPECIAL_EFFECT_LOCK)
	{
		if(eff->isSingleton() && !isAssignableFrom<IntrinsicAbility*>(eff))
		{
			std::string name = eff->getName();
			if(name.length() == 0) return;

			//sendMessage()
		}
	}
}