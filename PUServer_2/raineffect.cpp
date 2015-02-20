#include "raineffect.h"

#include "pokemon.h"
#include "battlefield.h"
#include "movelistentry.h"
#include "percenteffect.h"
#include "chargeeffect.h"
#include "paralysiseffect.h"

void RainEffect::modifySpeed( Pokemon* p, double factor )
{
	if(!p->hasAbility("Swift Swim")) {
		return;
	}

	p->getMultiplier(S_SPEED)->multiplyBy(factor);
}

bool RainEffect::apply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(m_applied[index] || !(m_applied[index] == hasEffects(p->getField())))
		return true;

	// Double the current effective Speed of pokemon with the
	// Swift Swim ability.
	modifySpeed(p, 2.0);
	
	pokemonTypeList_t list;
	list.push_back(PokemonType::T_WATER);
	setTypes(p, list, true);

	return true;
}

void RainEffect::unapply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(!m_applied[index])
		return;

	m_applied[index] = false;
	modifySpeed(p, 0.5);

	pokemonTypeList_t list;
	setTypes(p, list, false);
}

void RainEffect::tickWeather( BattleField* field )
{
	field->showMessage("The rain continues to fall.");
}

bool RainEffect::tickPokemon( Pokemon* p )
{
	if(!hasEffects(p->getField()))
		return false;

	if(p->hasAbility("Dry Skin")) {
		std::string message = boost::str(boost::format("%s soaked up rain!") % p->getName());
		p->getField()->showMessage(message);
		p->changeHealth(p->getStat(S_HP) / 8);
	}
	if(p->hasAbility("Hydration")) {
		if(p->hasEffect(SPECIAL_EFFECT_LOCK)) {
			std::string message = boost::str(boost::format("%s's status was cured!") % p->getName());
			p->getField()->showMessage(message);
			p->removeStatus(SPECIAL_EFFECT_LOCK);
		}
	}

	return false;
}

bool RainEffect::applyToField( BattleField* field )
{
	field->showMessage("Rain began to fall!");
	return true;
}

void RainEffect::unapplyToField( BattleField* field )
{
	field->showMessage("The rain stopped.");
}

MoveListEntry* RainEffect::getTransformedMove( Pokemon *p, MoveListEntry *entry )
{
	if(!hasEffects(p->getField()))
		return entry;

	PokemonMove* pmove = entry->getMove();
	PokemonType* type = pmove->getType();
	std::string name = entry->getName();

	if(type->equals(PokemonType::T_WATER)) {
		pmove->setPower((int)((double)pmove->getPower() * 1.5));
	}
	else if(type->equals(PokemonType::T_FIRE)) {
		pmove->setPower((int)((double)pmove->getPower() / 2.0));
	}
	else if(name == "Weather Ball") {
		pmove->setPower(100);
		pmove->setType(PokemonType::T_WATER);
	}
	else if(name == "Moonlight" || name == "Morning Sun" || name == "Synthesis") {
		StatusMove* statusMove = (StatusMove*)pmove;
		PercentEffect* perc = dynamic_cast<PercentEffect*>(statusMove->getEffects()[0]);
		perc->setPercent(1.0/3.0);
	}
	else if(name == "Solar Beam") {
		if(isAssignableFrom<StatusMove*>(pmove)) {
			StatusMove* statusMove = (StatusMove*)pmove;
			ChargeEffect* charge = dynamic_cast<ChargeEffect*>(statusMove->getEffects()[0]);
			charge->setTurns(2);
		}
	}
	else if(name == "Thunder")
	{	
		statusEffectList_t statuses;
		statuses.push_back(new ParalysisEffect());
		boolArray_t boolArray;
		boolArray.push_back(false);
		doubleArray_t doubleArray;
		doubleArray.push_back(0.3);

		return new MoveListEntry("Thunder", new RainEffectThunder(statuses, boolArray, doubleArray));
	}

	return entry;
}

bool RainEffectThunder::attemptHit( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	//TODO: return PerfectAccuracyMove::isHit(mech, attacker, defender);
	return false;
}