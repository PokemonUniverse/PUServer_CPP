#include "statcuteffect.h"

#include "pokemon.h"
#include "battlefield.h"
#include "movelistentry.h"
#include "statuseffect.h"

bool StatCutEffect::equals( StatusEffect* eff )
{
	if(!isAssignableFrom<StatCutEffect*>(eff)) return false;

	StatCutEffect* effect = dynamic_cast<StatCutEffect*>(eff);
	return ((m_stat == effect->getStat()) || (m_party == effect->getParty() ) );
}

bool StatCutEffect::applyToField( BattleField* field )
{
	Pokemon* user = field->getActivePokemon(m_party.m_team, m_party.m_position);
	m_turns = user->hasItem("Light Clay") ? 8 : 5;

	field->showMessage("A barrier was formed!");
	return true;
}

bool StatCutEffect::tickField( BattleField* field )
{
	if(m_turns-- <= 0) {
		Pokemon* user = field->getActivePokemon(m_party.m_team, m_party.m_position);
		field->showMessage(boost::str(boost::format("%s's %s wore off!") % user->getName() % m_name));
		field->removeEffect(this);

		return true;
	}

	return false;
}

MoveListEntry* StatCutEffect::getEnemyTransformedMove( Pokemon *p, MoveListEntry *move )
{
	teamInfo_s team(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(team != m_party) return move;

	PokemonMove* pmove = move->getMove();
	bool special = (m_stat == S_SPDEFENCE);
	if(pmove->isSpecial(p->getField()->getMechanics()) == special) {
		pmove->setPower(pmove->getPower() / 2);
	}

	return move;
}