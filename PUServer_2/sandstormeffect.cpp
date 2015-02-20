#include "sandstormeffect.h"

#include "battlefield.h"
#include "pokemon.h"
#include "pokemonmove.h"
#include "movelistentry.h"
#include "statmultiplier.h"
#include "jewelmechanics.h"
#include "chargeeffect.h"
#include "percenteffect.h"
#include "statusmove.h"

bool SandstormEffect::applyToField( BattleField* field )
{
	field->showMessage("A sandstorm brewed!");
	return true;
}

void SandstormEffect::unapplyToField( BattleField* field )
{
	field->showMessage("The sandstorm stopped.");
}

bool SandstormEffect::tickPokemon( Pokemon* p )
{
	if(!hasEffects(p->getField()))
		return false;

	if(p->isType(PokemonType::T_GROUND) || p->isType(PokemonType::T_ROCK) || p->isType(PokemonType::T_STEEL) || p->hasAbility("Sand Veil") )
		return false;

	int maximum = p->getStat(S_HP);
	int damage = maximum / 16;
	if(damage < 1) damage = 1;

	std::string message = boost::str(boost::format("%s is buffetted by the sandstorm!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-damage, true);

	return false;
}

bool SandstormEffect::apply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(m_applied[index] || !(m_applied[index] = hasEffects(p->getField())))
		return true;

	if(p->hasAbility("Sand Veil")) {
		p->getMultiplier(S_EVASION)->increaseMultiplier();
	}

	if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics()) && p->isType(PokemonType::T_ROCK)) {
		p->getMultiplier(S_SPDEFENCE)->multiplyBy(1.5);
	}

	return true;
}

void SandstormEffect::unapply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(!m_applied[index])
		return;

	m_applied[index] = false;
	if(p->hasAbility("Sand Veil")) {
		p->getMultiplier(S_EVASION)->decreaseMultiplier();
	}

	if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics()) && p->isType(PokemonType::T_ROCK)) {
		p->getMultiplier(S_SPDEFENCE)->divideBy(1.5);
	}
}

MoveListEntry* SandstormEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(!hasEffects(p->getField()))
		return move;

	PokemonMove* pmove = move->getMove();
	std::string name = move->getName();

	if(name == "Weather Ball") {
		pmove->setType(PokemonType::T_ROCK);
		pmove->setPower(100);
	}
	else if(name == "Solarbeam") {
		if(isAssignableFrom<StatusMove*>(pmove)) {
			StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
			ChargeEffect* charge = (ChargeEffect*)statusMove->getEffects()[0];
			charge->getMove()->getMove()->setPower(60);
		}
	}
	else if(name == "Moonlight" || name == "Morning Sun" || name == "Synthesis") 
	{
		StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
		PercentEffect* perc = (PercentEffect*)statusMove->getEffects()[0];
		perc->setPercent(1.0 / 3.0);
	}

	return move;
}

void SandstormEffect::tickWeather( BattleField* field )
{
	field->showMessage("The sandstorm rages.");
}