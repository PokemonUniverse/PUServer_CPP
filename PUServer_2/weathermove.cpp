#include "weathermove.h"

#include "pokemon.h"
#include "battlefield.h"
#include "weathereffect.h"

/*
template < class C >
uint32_t WeatherMove< C >::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	BattleField* field = attacker->getField();
	int length = attacker->hasItem(m_item) ? 8 : 5;

	WeatherEffect* eff = new C;
	eff->resetTurns(length);

	field->applyEffect(eff);

	return 0;
}*/


/*
uint32_t WeatherMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	BattleField* field = attacker->getField();
	int length = attacker->hasItem(m_item) ? 8 : 5;

	WeatherEffect* eff = new WeatherEffect(length);
	eff->resetTurns(length);

	field->applyEffect(eff);

	return 0;
}*/
