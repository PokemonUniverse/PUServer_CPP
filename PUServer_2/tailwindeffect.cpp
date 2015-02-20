#include "tailwindeffect.h"

#include "pokemon.h"
#include "battlefield.h"

bool TailwindEffect::equals( FieldEffect* eff )
{
	if(!isAssignableFrom<TailwindEffect*>(eff)) return false;

	TailwindEffect* effect = dynamic_cast<TailwindEffect*>(eff);
	return (m_party.m_team == effect->getParty().m_team);
}

bool TailwindEffect::applyToField( BattleField* field )
{
	field->showMessage("A tailwind picked up!");
	return true;
}

bool TailwindEffect::tickField( BattleField* field )
{
	if(m_turns-- <= 0) {
		field->showMessage("The tailwind died down.");
		field->removeEffect(this);
		return true;
	}

	return false;
}

bool TailwindEffect::apply( Pokemon* p )
{
	uint8_t idx = p->getMaster()->getBattleTeamIdx();
	if(idx == m_party.m_team) {
		p->getMultiplier(S_SPEED)->multiplyBy(2.0);
	}

	return FieldEffect::apply(p);
}

void TailwindEffect::unapply( Pokemon* p )
{
	uint8_t idx = p->getMaster()->getBattleTeamIdx();
	if(idx == m_party.m_team) {
		p->getMultiplier(S_SPEED)->divideBy(2.0);
	}

	FieldEffect::unapply(p);
}