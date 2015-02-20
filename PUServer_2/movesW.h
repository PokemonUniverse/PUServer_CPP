#ifndef INC_MOVESW_H_
#define INC_MOVESW_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "healthbasedmove.h"
#include "typecutmove.h"
#include "recoilmove.h"

#include "confuseeffect.h"
#include "burneffect.h"
#include "statchangeeffect.h"
#include "restrainingeffect.h"
#include "sleepeffect.h"

struct WakeupSlap : public PokemonMove
{
	WakeupSlap() : PokemonMove(PokemonType::T_FIGHTING, 60, 1.0, 10) {};
	
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = getPower();
		if(!defender->hasSubstitute() && defender->hasEffect<SleepEffect>()) {
			setPower(2 * power);
			defender->removeStatus(SPECIAL_EFFECT_LOCK);

			std::string message = boost::str(boost::format("%s woke up!") % defender->getName());
			attacker->getField()->showMessage(message);
		}

		int damage = mech->calculateDamage(this, attacker, defender);
		setPower(power);
		defender->changeHealth(-damage);
		return damage;
	}
};

struct WaterPulse : public StatusMove
{
	WaterPulse() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_WATER, 60, 1.0, 20, a1, a2, a3);
	};
};

struct WaterSport : public TypeCutMove
{
	WaterSport() : TypeCutMove(PokemonType::T_GROUND, 15, PokemonType::T_FIRE, "Water Sport") {};
};

struct WaterSpout : public HealthBasedMove
{
	WaterSpout() : HealthBasedMove(PokemonType::T_WATER, 1.0, 5, true) {};
};

struct Whirlpool : public StatusMove
{
	Whirlpool() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Whirlpool", "trapped in a vortex"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_WATER, 15, 0.7, 15, a1, a2, a3);
	};
};

struct Willowisp : public StatusMove
{
	Willowisp() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIRE, 0, 0.75, 15, a1, a2, a3);
	};
};

struct Withdraw : public StatusMove
{
	Withdraw() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_WATER, 0, 1.0, 40, a1, a2, a3);
	};
};

struct WoodHammer : public RecoilMove
{
	WoodHammer() : RecoilMove(PokemonType::T_GRASS, 120, 1.0, 15, 1.0/3.0) {};
};

struct WorrySeed : public PokemonMove
{
	WorrySeed() : PokemonMove(PokemonType::T_GRASS, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasAbility("Multitype") || defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		defender->setAbility(IntrinsicAbility::getInstance("Insomnia"), false);
		return 0;
	}
};

struct Wrap : public StatusMove
{
	Wrap() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Wrap", "wrapped"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 15, 0.85, 20, a1, a2, a3);
	};
};

#endif