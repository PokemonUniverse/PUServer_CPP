#include "haileffect.h"

#include "battlefield.h"
#include "pokemon.h"
#include "movelistentry.h"
#include "statusmove.h"
#include "chargeeffect.h"
#include "percenteffect.h"
#include "jewelmechanics.h"
#include "freezeeffect.h"

bool HailEffect::applyToField( BattleField* field )
{
	field->showMessage("Hail began to fall!");
	return true;
}

void HailEffect::unapplyToField( BattleField* field )
{
	field->showMessage("The hail stopped.");
}

bool HailEffect::tickPokemon( Pokemon* p )
{
	if(!hasEffects(p->getField()))
		return false;

	if(p->hasAbility("Ice Body")) {
		std::string message = boost::str(boost::format("%s absorbed the hail!") % p->getName());
		p->getField()->showMessage(message);
		p->changeHealth((int)floor(p->getStat(S_HP) / 16.));
		
		return false;
	}
	if(p->isType(PokemonType::T_ICE))
		return false;

	int maximum = p->getStat(S_HP);
	int damage = (int)floor(maximum / 16.);
	if(damage < 1) damage = 1;

	std::string message = boost::str(boost::format("%s is pelted by hail!") % p->getName());
	p->getField()->showMessage(message);
	p->changeHealth(-damage, true);

	return false;
}

MoveListEntry* HailEffect::getTransformedMove( Pokemon *p, MoveListEntry *move )
{
	if(!hasEffects(p->getField()))
		return move;

	PokemonMove* pmove = move->getMove();
	std::string name = move->getName();
	if(name == "Solarbeam") {
		if(isAssignableFrom<StatusMove*>(pmove)) 
		{
			StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
			ChargeEffect* charge = (ChargeEffect*)statusMove->getEffects()[0];
			charge->getMove()->getMove()->setPower(60);
		}
	}
	else if(name == "Weather Ball") {
		pmove->setPower(100);
		pmove->setType(PokemonType::T_ICE);
	}
	else if(name == "Moonlight" || name == "Morning Sun" || name == "Synthesis") 
	{
		StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
		PercentEffect* perc = (PercentEffect*)statusMove->getEffects()[0];
		perc->setPercent(1.0 / 3.0);
	}
	else if(name == "Blizzard") {
		if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics()))
		{
			statusEffectList_t statusList;
			statusList.push_back(new FreezeEffect());

			boolArray_t boolArray;
			boolArray.push_back(false);

			doubleArray_t doubleArray;
			doubleArray.push_back(0.1);

			return new MoveListEntry("Blizzard", new HailEffectBlizzard(statusList, boolArray, doubleArray));
		}
	}

	return move;
}

bool HailEffect::apply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(m_applied[index] || !(m_applied[index] = hasEffects(p->getField())) )
		return true;

	if(p->hasAbility("Snow Cloak")) {
		p->getMultiplier(S_EVASION)->increaseMultiplier();
	}

	pokemonTypeList_t list;
	list.push_back(PokemonType::T_ICE);
	setTypes(p, list, true);

	return true;
}

void HailEffect::unapply( Pokemon* p )
{
	teamInfo_s index(p->getMaster()->getBattleTeamIdx(), p->getMaster()->getBattleTeamPosition());
	if(!m_applied[index])
		return;

	m_applied[index] = false;
	if(p->hasAbility("Snow Cloak")) {
		p->getMultiplier(S_EVASION)->decreaseMultiplier();
	}

	pokemonTypeList_t list;
	setTypes(p, list, false);
}

void HailEffect::tickWeather( BattleField* field )
{
	field->showMessage("The hail continues to fall.");
}

bool HailEffectBlizzard::attemptHit( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	//TODO: return PerfectAccuracyMove::isHit(mech, user, target);
	return false;
}