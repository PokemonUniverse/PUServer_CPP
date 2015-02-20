#ifndef INC_MOVESG_H_
#define INC_MOVESG_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "absorbmove.h"
#include "onehitkillmove.h"
#include "massbasedmove.h"
#include "statchangeswapmove.h"
#include "pokemonmove.h"
#include "statuseffect.h"

#include "pokemon.h"
#include "statchangeeffect.h"
#include "jewelmechanics.h"
#include "sleepeffect.h"
#include "rechargeeffect.h"
#include "intrinsicability.h"
#include "gravityeffect.h"

struct GastroAcid : public PokemonMove
{
	GastroAcid() : PokemonMove(PokemonType::T_POISON, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		if(defender->hasSubstitute()) { 
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		IntrinsicAbility* ability = defender->getAbility();
		if(ability != NULL && ability->isActive()) {
			ability->unapply(defender);
			ability->deactivate();
		}

		std::string message = boost::str(boost::format("%s's ability was nullified.") % defender->getName());
		attacker->getField()->showMessage(message);

		defender->addStatus(attacker, new GastroAcidEffect());

		return 0;
	}

private:
	class GastroAcidEffect : public StatusEffect
	{
	public:
		GastroAcidEffect() {};
		std::string getName() { return "Gastro Acid"; };
	};
};

struct GigaDrain : public AbsorbMove
{
	GigaDrain() : AbsorbMove(PokemonType::T_GRASS, 60, 1.0, 10, 0.5) {};
};

struct GigaImpact : public StatusMove
{
	GigaImpact()
	{
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 150, 0.9, 5, a1, a2, a3);
	};
};

struct Glare : public StatusMove
{
	Glare() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.75, 30, a1, a2, a3);
	};

	bool isEffective(Pokemon* target) {
		if(!isAssignableFrom<JewelMechanics*>(target->getField()->getMechanics())) {
			return StatusMove::isEffective(target);
		}

		return true;
	}
};

struct GrassKnot : public MassBasedMove
{
	GrassKnot() : MassBasedMove(PokemonType::T_GRASS, 1.0, 20) {};
};

struct Grasswhistle : public StatusMove
{
	Grasswhistle() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 0.55, 15, a1, a2, a3);
	};
};

struct Gravity : public PokemonMove
{
	Gravity() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->applyEffect(new GravityEffect());
		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct Growl : public StatusMove
{
	Growl() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 40, a1, a2, a3);
	};
};

struct Growth : public StatusMove
{
	Growth() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 40, a1, a2, a3);
	};
};

struct Grudge : public StatusMove
{
	Grudge()
	{
		statusEffectList_t a1;
		a1.push_back(new GrudgeEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GHOST, 0, 1.0, 5, a1, a2, a3);
	};

private:
	class GrudgeEffect : public StatusEffect
	{
	public:
		GrudgeEffect() {};

		std::string getName() { return "Grudge"; };
		std::string getDescription() { return "is bearing a Grudge!"; };
		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };
		bool isListener() { return true; };
		bool hitsThroughSubstitude() { return true; };
		void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) {
			if(target->getHealth() <= 0) {
				for(int i = 0; i < 4; i++) {
					if(move->equals(attacker->getMove(i))) {
						std::string message = boost::str(boost::format("%s lost its PP due to the Grudge!") % move->getName());
						attacker->setPp(i, 0);
						break;
					}
				}
			}
		}

	};
};

struct GuardSwap : public StatChangeSwapMove
{
	GuardSwap()
	{
		intArray_t list;
		list.push_back(S_DEFENCE);
		list.push_back(S_SPDEFENCE);
		StatChangeSwapMove(PokemonType::T_PSYCHIC, 10, list);
	};
};

struct Guillotine : public OneHitKillMove
{
	Guillotine() : OneHitKillMove(PokemonType::T_NORMAL, 5) {};
};

struct GunkShot : public StatusMove
{
	GunkShot() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_POISON, 120, 0.7, 5, a1, a2, a3);
	};
};

struct GyroBall : public PokemonMove
{
	GyroBall() : PokemonMove(PokemonType::T_STEEL, 100, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = 51 * defender->getStat(S_SPEED) / attacker->getStat(S_SPEED) / 2;
		if(power > 150) power = 150;

		setPower((getPower() * power) / 100);
		int damage = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-damage);
		setPower(100);

		return damage;
	}
};

#endif