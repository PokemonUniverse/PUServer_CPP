#include "stockpilemove.h"

#include "statuseffect.h"
#include "statusmove.h"
#include "pokemon.h"
#include "stockpileeffect.h"

StockpileEffect* StockpileMove::getStockpileEffect( Pokemon* p )
{
	statusEffectList_t statuses = p->getNormalStatuses(0);
	statusEffectList_t::iterator it = statuses.begin();
	for(; it != statuses.end(); ++it)
	{
		StatusEffect* eff = (*it);
		if(isAssignableFrom<StockpileEffect*>(eff)) {
			return dynamic_cast<StockpileEffect*>(eff);
		}
	}

	return NULL;
}

int StockpileMove::getLevels( Pokemon* p )
{
	StockpileEffect* eff = getStockpileEffect(p);
	return (eff == NULL) ? -1 : eff->getLevels();
}