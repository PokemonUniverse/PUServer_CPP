#include "player.h"

Player::Player( const std::string& _name ) : Creature()
{
	m_name = _name;
	
	m_connection = NULL;
	m_isLoaded = false;

	m_money = 0;
}

void Player::assignConnection( Connection* _connection )
{
	if(m_connection == NULL) {
		m_connection = _connection;
	}	
}

void Player::removeConnection()
{
	delete m_connection;
	m_connection = NULL;
}

void Player::onCreatureAppear( Creature* creature, bool isLogin )
{
	bool canSeeCreature = canSee(creature->getPosition());
	if(!canSeeCreature)
		return;

	if(!knowsVisibleCreature(creature)) {
		addVisibleCreature(creature);
	}

	if(!creature->knowsVisibleCreature(this)) {
		creature->addVisibleCreature(this);
	}
}

void Player::onCreatureDisappear( Creature* creature, bool isLogout )
{
	bool canSeeCreature = canSee(creature->getPosition());
	if(canSeeCreature && knowsVisibleCreature(creature)) {
		removeVisibleCreature(creature);
	}
}

void Player::onCreatureMove( Creature* creature, Tile* from, Tile* to, bool teleport /*= false*/ )
{
	Creature::onCreatureMove(creature, from, to, teleport);

	if(creature == this) return;

	bool canSeeFromTile = canSee(from->getPosition());
	bool canSeeToTile = canSee(to->getPosition());

	if(!canSeeFromTile && !canSeeToTile) {
		return;
	}
	else if(canSeeToTile && !canSeeFromTile) // Creature enters viewport
	{
		if(!knowsVisibleCreature(creature)) {
			addVisibleCreature(creature);
		}

		if(!creature->knowsVisibleCreature(this)) {
			creature->addVisibleCreature(this);
		}

		sendCreatureMove(creature, from, to);
	}
	else if(!canSeeToTile && canSeeFromTile) // Creature leaves viewport
	{
		sendCreatureMove(creature, from, to);

		if(knowsVisibleCreature(creature))	{ 
			removeVisibleCreature(creature); 
		}
		if(creature->knowsVisibleCreature(this)) {
			creature->removeVisibleCreature(this);
		}
	}
	else // Creature moves in viewport
	{
		if(!knowsVisibleCreature(creature)) {
			addVisibleCreature(creature);
		}
		if(!creature->knowsVisibleCreature(this)) {
			creature->addVisibleCreature(this);
		}

		sendCreatureMove(creature, from, to);
	}
}

void Player::onCreatureTurn( Creature* creature, Direction dir )
{
	if(creature->getPlayer() == this) return;

	sendCreatureTurn(creature, dir);
}

void Player::removeVisibleCreature( Creature *creature )
{
	m_connection->sendRemoveVisibleCreature(creature);
	Creature::removeVisibleCreature(creature);
}

void Player::addVisibleCreature( Creature *creature )
{
	Creature::addVisibleCreature(creature);
	m_connection->sendAddVisibleCreature(creature);
}

int64_t Player::getLastActivityTime()
{
	if(m_connection == NULL) {
		return m_connectionLostTime;
	}

	return m_connection->getLastActivity();
}