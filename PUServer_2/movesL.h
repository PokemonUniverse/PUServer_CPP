#ifndef INC_MOVESL_H_
#define INC_MOVESL_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "absorbmove.h"
#include "highcriticalhitmove.h"
#include "massbasedmove.h"
#include "pokemonmove.h"
#include "partyeffect.h"

#include "pokemon.h"
#include "statuseffect.h"
#include "statchangeeffect.h"
#include "paralysiseffect.h"
#include "sleepeffect.h"
#include "leechseedeffect.h"
#include "statcuteffect.h"
#include "lockoneffect.h"

struct LastResort : public PokemonMove
{
	LastResort() : PokemonMove(PokemonType::T_NORMAL, 130, 1.0, 5) {};
	
	void switchIn(Pokemon *p) {
		intArray_t pp;

		for(int i = 0; i < 4; ++i) {
			pp[i] = p->getPp(i);
		}
		p->addStatus(p, new LastResortEffect(pp));
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		LastResortEffect* effect = (LastResortEffect*)attacker->getEffect<LastResortEffect>();
		if(effect == NULL) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		bool hasOtherMoves = false;
		for(int i = 0; i < 4; ++i)
		{
			MoveListEntry* entry = attacker->getMove(i);
			if(entry == NULL) continue;
			if(entry->getName() != "Last Resort")
			{
				hasOtherMoves = true;
				if(attacker->getPp(i) >= effect->getPp(i)) {
					hasOtherMoves = false;
					break;
				}
			}
		}

		if(!hasOtherMoves) { 
			attacker->getField()->showMessage("But if failed!");
			return 0;
		}

		return PokemonMove::use(mech, attacker, defender);
	};

private:
	class LastResortEffect : public StatusEffect
	{
		intArray_t m_pp;	
	public:
		LastResortEffect(intArray_t pp) : m_pp(pp) {};
		int getPp(int i) { return m_pp.at(i); };
	};
};

struct LavaPlume : public StatusMove
{
	LavaPlume() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FIRE, 80, 1.0, 15, a1, a2, a3);
	};
};

struct LeafBlade : public HighCriticalHitMove
{
	LeafBlade() : HighCriticalHitMove(PokemonType::T_GRASS, 70, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int power = (isAssignableFrom<JewelMechanics*>(mech)) ? 90 : 70;
		setPower(power);

		return HighCriticalHitMove::use(mech, defender, attacker);
	}
};

struct LeafStorm : public StatusMove
{
	LeafStorm() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 140, 0.9, 5, a1, a2, a3);
	};
};

struct LeechLife : public AbsorbMove
{
	LeechLife() : AbsorbMove(PokemonType::T_BUG, 20, 1.0, 15, 0.5) {};
};

struct LeechSeed : public StatusMove
{
	LeechSeed() {
		statusEffectList_t a1;
		a1.push_back(new LeechSeedEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 0.9, 10, a1, a2, a3);
	};
};

struct Leer : public StatusMove
{
	Leer() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	};
};

struct Lick : public StatusMove
{
	Lick() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_GHOST, 20, 1.0, 20, a1, a2, a3);
	};
};

struct LightScreen : public PokemonMove
{
	LightScreen() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		teamInfo_s team(attacker->getMaster()->getBattleTeamIdx(), attacker->getMaster()->getBattleTeamPosition());
		if(!attacker->getField()->applyEffect(new StatCutEffect(S_SPDEFENCE, team, "light screen"))) {
			attacker->getField()->showMessage("But it failed!");
		}

		return 0;
	}

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct LockOn : public PokemonMove
{
	LockOn() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->addStatus(attacker, new LockOnEffect());
		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct LovelyKiss : public StatusMove
{
	LovelyKiss() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.75, 10, a1, a2, a3);
	};
};

struct LowKick : public MassBasedMove
{
	LowKick() : MassBasedMove(PokemonType::T_FIGHTING, 1.0, 20) {};
};

struct LuckyChant : public PokemonMove
{
	LuckyChant() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 30) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		FieldEffect* effect = attacker->getField()->getEffectByType<LuckyChantEffect>();
		if(effect == NULL) {
			attacker->getField()->applyEffect(new LuckyChantEffect(" is feeling lucky!", "'s luck wore off..."));
		}

		effect = attacker->getField()->getEffectByType<LuckyChantEffect>();
		LuckyChantEffect* eff = dynamic_cast<LuckyChantEffect*>(effect);
		eff->activateParty(attacker);

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; }

private:
	class LuckyChantEffect : public PartyEffect
	{
	public:
		LuckyChantEffect(std::string start, std::string end) : PartyEffect(start, end) {};

		std::string getName() { return "Lucky chant"; };
	};
};

struct LunarDance : public PokemonMove
{
	LunarDance() : PokemonMove(PokemonType::T_PSYCHIC, 0, 0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		BattleField* field = attacker->getField();
		if(field->getAliveCount(attacker->getMaster()) == 1) {
			field->showMessage("But it failed!");
			return 0;
		}

		attacker->faint();
		field->requestAndWaitForSwitch(attacker->getMaster());
		Pokemon* target = field->getActivePokemon(attacker->getMaster());
		target->changeHealth(target->getStat(S_HP));

		for(int i = 0; i < 4; ++i)
		{
			MoveListEntry* entry = target->getMove(i);
			if(entry != NULL && entry->getMove() != NULL) {
				PokemonMove* move = entry->getMove();
				target->setPp(i, target->getMaxPp(i));
			}
		}

		target->removeStatus(SPECIAL_EFFECT_LOCK);
		return 0;
	}

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	}
};

struct LusterPurge : public StatusMove
{
	LusterPurge() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.5);

		StatusMove(PokemonType::T_PSYCHIC, 0, 0.75, 10, a1, a2, a3);
	};
};

#endif