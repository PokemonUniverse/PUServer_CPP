#include "creature.h"
#include "battleteam.h"
#include "pokemonparty.h"

pokemonList_t BattleTeam::getActivePokemonList()
{
	pokemonList_t list;

	return list;
}

Pokemon* BattleTeam::getActivePokemon( int idx )
{
	return NULL;
}

Pokemon* BattleTeam::getActivePokemon()
{
	Creature* c = m_team.at(m_active);
	return c->getPokemonParty()->getActivePokemon();
}

void BattleTeam::attachField( BattleField* field, uint8_t team )
{
	m_id = team;
	for(int i = 0; i < m_team.size(); ++i) {
		Creature* c = m_team.at(i);
		if(c != NULL) {
			c->attachToField(field, m_id, i);
		}
	}
}