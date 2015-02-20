#ifndef INC_MOVESA_H_
#define INC_MOVESA_H_

#include "definitions.h"
#include "statusmove.h"
#include "multiplehitmove.h"
#include "absorbmove.h"
#include "highcriticalhitmove.h"
#include "perfectaccuracymove.h"
#include "prioritymove.h"

#include "statchangeeffect.h"
#include "flincheffect.h"
#include "attracteffect.h"
#include "multiplestatchangeeffect.h"
#include "assuranceeffect.h"

struct Absorb : public AbsorbMove
{
	Absorb() : AbsorbMove(PokemonType::T_GRASS, 20, 1.0, 20, 0.5) {};
};

struct Acid : public StatusMove
{
	Acid() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_POISON, 40, 1.0, 30, a1, a2, a3);
	};
};

struct AcidArmor : public StatusMove
{
	AcidArmor() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_POISON, 0, 1.0, 40, a1, a2, a3);
	};
};

struct Acupressure : public PokemonMove
{
	Acupressure() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 30) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int random = random_range(0, 7);
		attacker->addStatus(attacker, new StatChangeEffect(random, true, 2));
		
		return 0;
	}
};

struct AerialAce : public PerfectAccuracyMove
{
	AerialAce() : PerfectAccuracyMove(PokemonType::T_FLYING, 60, 20) {};
};

struct Aeroblast : public HighCriticalHitMove
{
	Aeroblast() : HighCriticalHitMove(PokemonType::T_FLYING, 120, 0.95, 5) {};
};

struct Agility : public StatusMove
{
	Agility() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 30, a1, a2, a3);
	};
};

struct AirCutter : public HighCriticalHitMove
{
	AirCutter() : HighCriticalHitMove(PokemonType::T_FLYING, 55, 0.95, 25) {};
};

struct AirSlash : public StatusMove
{
	AirSlash() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FLYING, 75, 0.95, 20, a1, a2, a3);
	};
};

struct Amnesia : public StatusMove
{
	Amnesia() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Ancientpower : public StatusMove
{
	Ancientpower() {
		intArray_t stats;
		stats.push_back(S_ATTACK);
		stats.push_back(S_DEFENCE);
		stats.push_back(S_SPEED);
		stats.push_back(S_SPATTACK);
		stats.push_back(S_SPDEFENCE);
		
		statusEffectList_t a1;
		a1.push_back(new MultipleStatChangeEffect(stats));

		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ROCK, 60, 1.0, 5, a1, a2, a3);
	};
};

struct AquaJet : public PriorityMove
{
	AquaJet() : PriorityMove(PokemonType::T_WATER, 40, 1.0, 30, 1) {};
};

struct AquaRing : public StatusMove
{
	AquaRing()
	{
		statusEffectList_t a1;
		a1.push_back(new AquaRingEffect());

		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_WATER, 0, 1.0, 20, a1, a2, a3);
	};

private:
	class AquaRingEffect : public PercentEffect
	{
	public:
		AquaRingEffect() : PercentEffect(0.0625, false, 3, "'s Aqua Ring restored health!") {};
		bool isSingleton() { return true; };
	};
};

struct AquaTail : public PokemonMove
{
	AquaTail() : PokemonMove(PokemonType::T_WATER, 90, 0.9, 10) {};
};

struct ArmThrust : public MultipleHitMove
{
	ArmThrust() : MultipleHitMove(PokemonType::T_FIGHTING, 15, 1.0, 20) {};
};

struct Assist : public PokemonMove
{
	Assist() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		pokemonList_t teammates = attacker->getTeammates();
		if(teammates.size() < 2) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		Pokemon* teammate;
		do 
		{
			teammate = teammates.at(random_range(0, teammates.size()));
		} while (teammate == NULL || teammate == attacker);

		MoveListEntry* move;
		do 
		{
			move = teammate->getMove(random_range(0,3));
		} while (move == NULL);

		if(move->getName() == "Focus Punch") {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return attacker->useMove(move, defender);
	}
};

struct Assurance : public PokemonMove
{
	Assurance() : PokemonMove(PokemonType::T_DARK, 50, 1.0, 10) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source) {
		source->getOpponent()->addStatus(source, new AssuranceEffect());
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		StatusEffect* effect = defender->getEffect<AssuranceEffect>();
		if(effect == NULL) return PokemonMove::use(mech, attacker, defender);

		AssuranceEffect* eff = dynamic_cast<AssuranceEffect*>(effect);
		int power = getPower();
		if(defender->getHealth() < eff->getHealth()) {
			setPower(power * 2);
		}
		
		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(power);

		return damage;
	};
};

struct Astonish : public StatusMove
{
	Astonish() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_GHOST, 30, 1.0, 15, a1, a2, a3);
	};
};

struct AttackOrder : public HighCriticalHitMove
{
	AttackOrder() : HighCriticalHitMove(PokemonType::T_BUG, 90, 1.0, 15) {};
};

struct Attract : public StatusMove
{
	Attract() {
		statusEffectList_t a1;
		a1.push_back(new AttractEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 15, a1, a2, a3);
	};
};

struct AuroraBeam : public StatusMove
{
	AuroraBeam() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ICE, 65, 1.0, 20, a1, a2, a3);
	};
};

struct AuraSphere : public PerfectAccuracyMove
{
	AuraSphere() : PerfectAccuracyMove(PokemonType::T_FIGHTING, 90, 20) {};
};
#endif