#ifndef INC_MOVESC_H_
#define INC_MOVESC_H_

#include "definitions.h"
#include "statusmove.h"
#include "multiplehitmove.h"
#include "highcriticalhitmove.h"
#include "statuseffect.h"
#include "countermove.h"

#include "pokemon.h"
#include "pokemontype.h"
#include "statchangeeffect.h"
#include "confuseeffect.h"
#include "conversioneffect.h"
#include "restrainingeffect.h"
#include "jewelmechanics.h"
#include "movelistentry.h"
#include "poisoneffect.h"

struct CalmMind : public StatusMove
{
	CalmMind() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true));
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, true));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Camouflage : public StatusMove
{
	Camouflage() {
		statusEffectList_t a1;
		a1.push_back(new MyCamouflage());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};

private:
	class MyCamouflage : public StatusEffect
	{
		pokemonTypeList_t m_types;

	public:
		MyCamouflage() {};

		std::string getName() { return "Camouflage"; };
		std::string getDescription() { return ""; };

		bool apply(Pokemon* p) {
			m_types = p->getTypes();

			//TODO: This should be based on the terrain but it isn't implemented yet
			PokemonType* type = PokemonType::T_NORMAL;

			pokemonTypeList_t list;
			list.push_back(type);
			p->setType(list);

			std::string message = boost::str(boost::format("%s became the %s type!") % p->getName() % type->toString());
			p->getField()->showMessage(message);

			return true;
		};

		int getTier() { return -1; };

		bool tick(Pokemon* p) { return false; };

		bool switchOut(Pokemon* p) {
			p->setType(m_types);
			return true;
		}
	};
};

struct Captivate : public StatusMove
{
	Captivate() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int attackerGender = attacker->getGender();
		int defenderGender = defender->getGender();
		if(attackerGender == defenderGender || attackerGender == GENDER_NONE || defenderGender == GENDER_NONE) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return StatusMove::use(mech, attacker, defender);
	};
};

struct Charge : public StatusMove
{
	Charge() {
		statusEffectList_t a1;
		a1.push_back(new MyCharge());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 20, a1, a2, a3);
	};

private:
	class MyCharge : public StatusEffect
	{
		uint32_t m_turns;

	public:
		MyCharge() { m_turns = 2; };

		std::string getName() { return "Charge"; };
		std::string getDescription() { return "is charging power!"; };

		bool apply(Pokemon* p) {
			if(isAssignableFrom<JewelMechanics*>(p->getField()->getMechanics())) {
				p->addStatus(p, new StatChangeEffect(S_SPDEFENCE, true));
			}

			return true;
		};

		int getTier() { return 1; };

		bool tick(Pokemon* p) {
			if(--m_turns <= 0) {
				p->removeStatus(this);
				return true;
			}

			return false;
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
			PokemonMove* pmove = move->getMove();
			if(pmove->getType()->equals(PokemonType::T_ELECTRIC)) {
				int power = pmove->getPower();
				pmove->setPower(power * 2);
			}

			return move;
		}
	};
};

struct ChargeBeam : public StatusMove
{
	ChargeBeam() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.7);

		StatusMove(PokemonType::T_ELECTRIC, 50, 0.9, 10, a1, a2, a3);
	};
};

struct Charm : public StatusMove
{
	Charm() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Chatter : public StatusMove
{
	Chatter() 
	{
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.31);

		StatusMove(PokemonType::T_FLYING, 60, 1.0, 20, a1, a2, a3);
	};
};

struct Clamp : public StatusMove
{
	Clamp() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Clamp", "clamped"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_WATER, 35, 0.75, 10, a1, a2, a3);
	};
};

struct CloseCombat : public StatusMove
{
	CloseCombat() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_FIGHTING, 120, 1.0, 5, a1, a2, a3);
	}
};

struct CometPunch : public MultipleHitMove
{
	CometPunch() : MultipleHitMove(PokemonType::T_NORMAL, 18, 0.85, 15) {};
};

struct Constrict : public StatusMove
{
	Constrict() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 10, 1.0, 35, a1, a2, a3);
	};
};

struct ConfuseRay : public StatusMove
{
	ConfuseRay() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GHOST, 0, 1.0, 10, a1, a2, a3);
	};
};

struct Confusion : public StatusMove
{
	Confusion() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_PSYCHIC, 50, 1.0, 25, a1, a2, a3);
	};
};

struct Conversion : public StatusMove
{
	Conversion() {
		statusEffectList_t a1;
		a1.push_back(new ConversionEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	};
};

struct Conversion2 : public StatusMove
{
	Conversion2() {
		statusEffectList_t a1;
		a1.push_back(new Conversion2Effect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	}
};

struct CosmicPower : public StatusMove
{
	CosmicPower() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, true));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 20, a1, a2, a3);
	};
};

struct CottonSpore : public StatusMove
{
	CottonSpore() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 0.85, 40, a1, a2, a3);
	};
};

struct Counter : public CounterMove
{
	Counter() : CounterMove(PokemonType::T_FIGHTING, 1.0, 20, 2) {};
};

struct Crabhammer : public HighCriticalHitMove
{
	Crabhammer() : HighCriticalHitMove(PokemonType::T_WATER, 90, 0.85, 10) {};
};

struct CrossChop : public HighCriticalHitMove
{
	CrossChop() : HighCriticalHitMove(PokemonType::T_FIGHTING, 100, 0.8, 5) {};
};

struct CrossPoison : public StatusMove
{
	CrossPoison() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_POISON, 70, 1.0, 20, a1, a2, a3);
	};

	bool hasHighCriticalHitRate() { return true; };
};

struct Crunch : public StatusMove
{
	Crunch() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_DARK, 80, 1.0, 15, a1, a2, a3);
	};
};

struct CrushClaw : public StatusMove
{
	CrushClaw() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_NORMAL, 75, 0.95, 10, a1, a2, a3);
	};
};

struct CrushGrip : public PokemonMove
{
	CrushGrip() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = (int)(110.0 * (((double)defender->getHealth()) / ((double)defender->getStat(S_HP) ) ) );
		setPower(power);
		int damage = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-damage);
		return damage;
	};

	bool isAttack() { return true; };
};

struct Curse : public PokemonMove
{
	Curse() : PokemonMove(PokemonType::T_TYPELESS, 0, 1.0, 10) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(attacker->isType(PokemonType::T_GHOST)) 
		{
			attacker->addStatus(attacker, new PercentEffect(-0.5, false, -1, ""));
			defender->addStatus(attacker, new PercentEffect(-0.25, false, 3, "is afflicted by the curse!"));

			std::string message = boost::str(boost::format("%s cuts its health and laid a curse on %s") % attacker->getName() % defender->getName());
			attacker->getField()->showMessage(message);
		}
		else {
			attacker->addStatus(attacker, new StatChangeEffect(S_SPEED, false, 1));
			attacker->addStatus(attacker, new StatChangeEffect(S_ATTACK, true, 1));
			attacker->addStatus(attacker, new StatChangeEffect(S_DEFENCE, true, 1));
		}

		return 0;
	}
};
#endif