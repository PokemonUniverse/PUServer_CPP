#include "statchangeswapmove.h"

#include "statuseffect.h"
#include "pokemon.h"
#include "statchangeeffect.h"

statusEffectList_t StatChangeSwapMove::cleanList(statusEffectList_t list)
{
	statusEffectList_t::iterator it = list.begin();
	while(it != list.end())
	{
		StatusEffect* eff = (StatusEffect*)(*it);
		if(eff == NULL) continue;

		if(!isAssignableFrom<StatChangeEffect*>(eff)) {
			it = list.erase(it);
			continue;
		}

		StatChangeEffect* effect = dynamic_cast<StatChangeEffect*>(eff);
		int stat = effect->getStat();
		bool good = false;
		for(int i = 0; i < m_stats.size(); ++i) {
			if(stat == m_stats[i]) {
				good = true;
			}
		}

		if(!good) { it = list.erase(it); }
	}

	return list;
}

void StatChangeSwapMove::addStatuses( Pokemon *p, statusEffectList_t effects )
{
	statusEffectList_t::iterator it = effects.begin();
	for(; it != effects.end(); ++it)
	{
		StatChangeEffect* eff = (StatChangeEffect*)(*it);
		eff->setDescription("");

		p->addStatus(p, eff);
	}
}

void StatChangeSwapMove::removeStatuses( Pokemon *p, statusEffectList_t effects )
{
	statusEffectList_t::iterator it = effects.begin();
	for(; it != effects.end(); ++it)
	{
		StatChangeEffect* eff = (StatChangeEffect*)(*it);
		p->removeStatus(eff);
	}
}

uint32_t StatChangeSwapMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	statusEffectList_t attackerStatuses = cleanList(attacker->getNormalStatuses(0));
	statusEffectList_t defenderStatuses = cleanList(defender->getNormalStatuses(0));

	removeStatuses(attacker, attackerStatuses);
	removeStatuses(defender, defenderStatuses);

	addStatuses(attacker, defenderStatuses);
	addStatuses(defender, attackerStatuses);

	attacker->getField()->showMessage("The Pokemon swapped stats!");

	return 0;
}