#include "creature.h"
#include "player.h"
#include "npc.h"
#include "pokemon.h"
#include "pokemonparty.h"
#include "battlefield.h"
#include "battlemechanics.h"

boost::recursive_mutex AutoID::autoIDLock;
uint32_t AutoID::count = 1000;
AutoID::list_type AutoID::list;

Creature::Creature()
{
	m_outfit = new Outfit();

	m_id = auto_id;
	m_tile = NULL;
	m_direction = DIR_SOUTH;

	m_lastStep = PUSYS_TIME();
	m_walkSpeed = 280;
	m_movement = MOVEMENT_WALK;

	m_field = NULL;
	isInternalRemoved = false;
}

Creature::~Creature()
{

}

void Creature::attachToField( BattleField* field, uint8_t team, uint8_t pos )
{
	if(m_creatureType == POKEMON) return; 

	m_field = field;
	m_mech = field->getMechanics();
	m_battleTeam = team;
	m_battleTeamPos = pos;

	PokemonParty::iterator it = m_pokeParty->begin();
	for(; it != m_pokeParty->end(); ++it) {
		Pokemon* p = (*it).second;
		p->attachToField(false);
	}
}

uint64_t Creature::getTimeSinceLastMove() const
{
	if(m_lastStep) {
		return PUSYS_TIME() - m_lastStep;
	}

	return 0x7FFFFFFFFFFFFFFFLL;
}

void Creature::onCreatureMove( Creature* creature, Tile* from, Tile* to, bool teleport /*= false*/ )
{
	if(creature == this) {
		m_lastStep = PUSYS_TIME();
	}
}

bool Creature::canMove()
{
	uint64_t lastmove = getTimeSinceLastMove();
	//std::cout << "Move speed = " << lastmove << std::endl;
	return (lastmove >= m_walkSpeed) ? true : false;
}

bool Creature::canSee(Creature *creature)
{
	if(!creature) return false;
	return canSee(creature->getPosition());
}

bool Creature::canSee(Position &pos)
{
	Position myPos = getPosition();
	if(myPos.z != pos.z) {
		return false;
	}
	if(!Position::areInRange<VIEWPORT_X_CENTER, VIEWPORT_Y_CENTER, 0>(myPos, pos)) {
		return false;
	}

	return true;
}

void Creature::removeVisibleCreature( Creature *creature )
{
	CreatureList::iterator find = std::find(m_visibleCreatures.begin(), m_visibleCreatures.end(), creature);
	if(find != m_visibleCreatures.end()) {
		m_visibleCreatures.erase(find);
	}
}

void Creature::addVisibleCreature( Creature *creature )
{
	CreatureList::iterator find = std::find(m_visibleCreatures.begin(), m_visibleCreatures.end(), creature);
	if(find == m_visibleCreatures.end()) {
		m_visibleCreatures.push_back(creature);
	}
}

bool Creature::knowsVisibleCreature( Creature *creature )
{
	if(m_visibleCreatures.empty()) return false;

	CreatureList::iterator find = std::find(m_visibleCreatures.begin(), m_visibleCreatures.end(), creature);
	if(find != m_visibleCreatures.end()) {
		return true;
	}

	return false;
}