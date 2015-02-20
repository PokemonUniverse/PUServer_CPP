#include "suneffect.h"

#include "pokemon.h"
#include "movelistentry.h"
#include "pokemontype.h"
#include "statusmove.h"
#include "chargeeffect.h"
#include "percenteffect.h"
#include "statmultiplier.h"

bool SunEffect::apply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(m_applied[index] || !(m_applied[index] = hasEffects(p->getField())))
		return true;

	modifySpeed(p, 2.0);
	modifySpAttack(p, 1.5);
	flowerGift(p, true);

	pokemonTypeList_t list;
	list.push_back(PokemonType::T_FIRE);
	setTypes(p, list, true);

	return true;
}

void SunEffect::unapply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(!m_applied[index])
		return;

	m_applied[index] = false;

	modifySpeed(p, 0.5);
	modifySpAttack(p, 2.0 / 3.0);
	flowerGift(p, false);

	pokemonTypeList_t list;
	setTypes(p, list, false);
}

bool SunEffect::tickPokemon( Pokemon* p )
{
	if(!hasEffects(p->getField()))
		return false;

	if(p->hasAbility("Dry Skin") || p->hasAbility("Solar Power")) {
		std::string message = boost::str(boost::format("%s was hurt by the sunlight!") % p->getName());
		p->getField()->showMessage(message);
		p->changeHealth(-p->getStat(S_HP) / 8, true);
	}

	return false;
}

void SunEffect::unapplyToField( BattleField* field )
{
	field->showMessage("The sun faded.");
}

bool SunEffect::applyToField( BattleField* field )
{
	field->showMessage("The sun began to shine!");
	return true;
}

MoveListEntry* SunEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(!hasEffects(p->getField()))
		return move;

	PokemonMove* pmove = move->getMove();
	PokemonType* type = pmove->getType();
	std::string name = move->getName();

	if(type->equals(PokemonType::T_FIRE)) {
		pmove->setPower((int)((double)pmove->getPower() * 1.5));
	}
	else if(type->equals(PokemonType::T_WATER)) {
		pmove->setPower((int)((double)pmove->getPower() / 2.0));
	}
	else if(name == "Solarbeam") {
		if(isAssignableFrom<StatusMove*>(pmove)) {
			StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
			// Assume first effect is ChargeEffect!
			ChargeEffect* charge = dynamic_cast<ChargeEffect*>(statusMove->getEffects()[0]);
			charge->setTurns(0);
		}
	}
	else if(name == "Thunder") {
		pmove->setAccuracy(0.5);
	}
	else if(name == "Weather Ball") {
		pmove->setPower(100);
		pmove->setType(PokemonType::T_FIRE);
	}
	else if(name == "Moonlight" || name == "Morning Sun" || name == "Synthesis") {
		if(isAssignableFrom<StatusMove*>(pmove)) {
			StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
			// Assume first effect is PercentEffect!
			PercentEffect* perc = dynamic_cast<PercentEffect*>(statusMove->getEffects()[0]);
			perc->setPercent(2.0/3.0);
		}
	}

	return move;
}

void SunEffect::tickWeather( BattleField* field )
{
	field->showMessage("The sun continues to shine.");
}

void SunEffect::modifySpeed( Pokemon* p, double factor )
{
	if(!p->hasAbility("Chlorophyll")) {
		return;
	}

	p->getMultiplier(S_SPEED)->multiplyBy(factor);
}

void SunEffect::modifySpAttack( Pokemon* p, double factor )
{
	if(!p->hasAbility("Solar Power")) {
		return;
	}

	p->getMultiplier(S_SPATTACK)->multiplyBy(factor);
}

void SunEffect::flowerGift( Pokemon* p, bool increase )
{
	if(!p->hasAbility("Flower Gift")) {
		return;
	}

	StatMultiplier* multiplier = p->getMultiplier(S_ATTACK);
	if(increase) { 
		multiplier->increaseMultiplier();
	} else {
		multiplier->decreaseMultiplier();
	}

	multiplier = p->getMultiplier(S_SPDEFENCE);
	if(increase) {
		multiplier->increaseMultiplier();
	} else {
		multiplier->decreaseMultiplier();
	}
}