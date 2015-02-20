#include "damagelistenermove.h"

#include "pokemon.h"
#include "damagelistenereffect.h"

void DamageListenerMove::beginTurn( battleTurnList_t turn, int index, Pokemon *source )
{
	source->addStatus(source, new DamageListenerEffect());
}

DamageListenerEffect* DamageListenerMove::getListener( Pokemon* p )
{
	DamageListenerEffect* listener = NULL;
	statusEffectList_t effects = p->getNormalStatuses(0);
	statusEffectList_t::iterator it = effects.begin();
	for(; it != effects.end(); ++it) {
		if(isAssignableFrom<DamageListenerEffect*>((*it))) {
			listener = dynamic_cast<DamageListenerEffect*>((*it));
			break;
		}
	}

	return listener;
}