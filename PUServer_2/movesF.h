#ifndef INC_MOVESF_H_
#define INC_MOVESF_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "multiplehitmove.h"
#include "healthbasedmove.h"
#include "perfectaccuracymove.h"
#include "pokemonmove.h"
#include "onehitkillmove.h"
#include "damagelistenermove.h"
#include "recoilmove.h"

#include "jewelmechanics.h"
#include "statchangeeffect.h"
#include "confuseeffect.h"
#include "burneffect.h"
#include "restrainingeffect.h"
#include "rechargeeffect.h"
#include "delayeddamageeffect.h"
#include "damagelistenereffect.h"
#include "paralysiseffect.h"
#include "countereffect.h"
#include "furycuttereffect.h"

class Pokemon;
class BattleMechanics;

struct Facade : public PokemonMove
{
	Facade() : PokemonMove(PokemonType::T_NORMAL, 70, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = getPower();
		if(attacker->hasEffect(SPECIAL_EFFECT_LOCK)) {
			setPower(2 * power);
		}

		int damage = mech->calculateDamage(this, attacker, defender);
		setPower(power);
		defender->changeHealth(-damage);

		return damage;
	}
};

struct FaintAttack : public PerfectAccuracyMove
{
	FaintAttack() : PerfectAccuracyMove(PokemonType::T_DARK, 60, 20) {};
};

struct FalseSwipe : public PokemonMove
{
	FalseSwipe() : PokemonMove(PokemonType::T_NORMAL, 70, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int targetHealth = defender->getHealth();
		int damage = mech->calculateDamage(this, attacker, defender);
		if(damage >= targetHealth) {
			damage = targetHealth - 1;
		}
		defender->changeHealth(-damage);

		return damage;
	}
};

struct FakeOut : public PokemonMove
{
	FakeOut() : PokemonMove(PokemonType::T_NORMAL, 40, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(!attacker->isFirstTurn()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		if((getEffectiveness(attacker, defender) != 0.0) && !defender->hasSubstitute()) {
			defender->addStatus(attacker, new FlinchEffect());
		}

		return PokemonMove::use(mech, attacker, defender);
	};

	int getPriority() { return 1; };
};

struct FakeTears : public StatusMove
{
	FakeTears() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Featherdance : public StatusMove
{
	Featherdance() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FLYING, 0, 1.0, 15, a1, a2, a3);
	};
};

struct Feint : public PokemonMove
{
	Feint() : PokemonMove(PokemonType::T_NORMAL, 50, 1.0, 10) {};
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(!defender->hasEffect<CounterEffect>()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return PokemonMove::use(mech, attacker, defender);
	}
};

struct FireBlast : public StatusMove
{
	FireBlast() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FIRE, 120, 0.85, 5, a1, a2, a3);
	};
};

struct FireFang : public StatusMove
{
	FireFang() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		a1.push_back(new BurnEffect());
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 0.1);

		StatusMove(PokemonType::T_FIRE, 65, 0.95, 15, a1, a2, a3);
	};
};

struct FirePunch : public StatusMove
{
	FirePunch() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIRE, 75, 1.0, 15, a1, a2, a3);
	};
};

struct FireSpin : public StatusMove
{
	FireSpin() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Fire Spin", "trapped in a vortex"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIRE, 15, 0.7, 15, a1, a2, a3);
	};
};

struct Fissure : public OneHitKillMove
{
	Fissure() : OneHitKillMove(PokemonType::T_GROUND, 5) {};
};

struct Flail : public HealthBasedMove
{
	Flail() : HealthBasedMove(PokemonType::T_NORMAL, 1.0, 15, false) {};
};

struct Flamethrower : public StatusMove
{
	Flamethrower() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIRE, 95, 1.0, 15, a1, a2, a3);
	};
};

struct FlameWheel : public StatusMove
{
	FlameWheel() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIRE, 60, 1.0, 25, a1, a2, a3);
	};
};

struct FlareBlitz : public RecoilMove
{
	FlareBlitz() : RecoilMove(PokemonType::T_FIRE, 120, 1.0, 15, 1.0/3.0) {};
};

struct Flash : public StatusMove
{
	Flash() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.7, 20, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		double accuracy = (isAssignableFrom<JewelMechanics*>(mech)) ? 1.0 : 0.7;
		setAccuracy(accuracy);

		return StatusMove::attemptHit(mech, attacker, defender);
	}
};

struct FlashCannon : public StatusMove
{
	FlashCannon() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_STEEL, 80, 1.0, 10, a1, a2, a3);
	}
};

struct Flatter : public StatusMove
{
	Flatter() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true, 2));
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 15, a1, a2, a3);
	};
};

struct Fly : public StatusMove
{
	Fly() {
		std::vector<std::string>list;
		list.push_back("Thunder");
		list.push_back("Twister");
		list.push_back("Gust");
		list.push_back("Sky Uppercut");

		statusEffectList_t a1;
		a1.push_back(new InvulnerableStateEffect(list));
		a1.push_back(new ChargeEffect(1, "flew up high!", new MoveListEntry("Fly", new PokemonMove(PokemonType::T_FLYING, 70, 0.95, 15) ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_FLYING, 0, 0.95, 15, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct FocusBlast : public StatusMove
{
	FocusBlast() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIGHTING, 120, 0.7, 5, a1, a2, a3);
	}
};

struct FocusPunch : public DamageListenerMove
{
	FocusPunch() : DamageListenerMove(PokemonType::T_FIGHTING, 150, 1.0, 20) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source) {
		if(!source->hasEffect<SleepEffect>() && !source->hasEffect<FreezeEffect>()) {
			std::string message = boost::str(boost::format("%s is tightening its focus!") % source->getName());
			source->getField()->showMessage(message);
		}

		return DamageListenerMove::beginTurn(turn, index, source);
	}

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		DamageListenerEffect* listener = getListener(attacker);
		if(listener == NULL) return 0;

		if(listener->getDamage() > 0) {
			std::string message = boost::str(boost::format("%s lost its focus and couldn't move!") % attacker->getName());
			attacker->getField()->showMessage(message);

			return 0;
		}

		return DamageListenerMove::use(mech, attacker, defender);
	}

	int getPriority() { return -2; };
};

// Doens't do anything in 1 vs 1
struct FollowMe : public PokemonMove
{
	FollowMe() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		std::string message = boost::str(boost::format("%s became the centre of attention!") % attacker->getName());
		attacker->getField()->showMessage(message);
		return 0;
	}

	int getPriority() { return 4; };
};

struct ForcePalm : public StatusMove
{
	ForcePalm()
	{
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FIGHTING, 60, 1.0, 10, a1, a2, a3);
	};
};

struct Foresight : public PokemonMove
{
	Foresight() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 40) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		statusEffectList_t statuses = defender->getNormalStatuses(0);
		statusEffectList_t::iterator it = statuses.begin();
		for(; it != statuses.end(); ++it) {
			StatusEffect* effect = (*it);
			if(!isAssignableFrom<StatChangeEffect*>(effect)) continue;

			StatChangeEffect* eff = dynamic_cast<StatChangeEffect*>(effect);
			if(eff->getStat() == S_EVASION) {
				defender->removeStatus(eff);
			}
		}

		statuses = attacker->getNormalStatuses(0);
		it = statuses.begin();
		for(; it != statuses.end(); ++it) {
			StatusEffect* effect = (*it);
			if(!isAssignableFrom<StatChangeEffect*>(effect)) continue;

			StatChangeEffect* eff = dynamic_cast<StatChangeEffect*>(effect);
			if(eff->getStat() == S_ACCURACY) {
				attacker->removeStatus(eff);
			}
		}

		attacker->getAccuracy()->setSecondMultiplier(1.0);
		defender->getEvasion()->setSecondMultiplier(1.0);

		return 0;
	};
};

struct FrenzyPlant : public StatusMove
{
	FrenzyPlant() {
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 150, 0.9, 5, a1, a2, a3);
	};
};

struct FuryAttack : public MultipleHitMove
{
	FuryAttack() : MultipleHitMove(PokemonType::T_NORMAL, 15, 0.85, 20) {};
};

struct FuryCutter : public StatusMove
{
	FuryCutter() {
		statusEffectList_t a1;
		a1.push_back(new FuryCutterEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_BUG, 10, 0.95, 20, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(!StatusMove::attemptHit(mech, attacker, defender)) {
			attacker->removeStatus<FuryCutterEffect>();
		}

		return StatusMove::use(mech, attacker, defender);
	};
};

struct FurySwipes : public MultipleHitMove
{
	FurySwipes() : MultipleHitMove(PokemonType::T_NORMAL, 18, 0.8, 15) {};
};

struct FutureSight : public PokemonMove
{
	FutureSight() : PokemonMove(PokemonType::T_PSYCHIC, 80, 0.9, 15) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage;
		if(!PokemonMove::attemptHit(mech, attacker, defender)) {
			damage = 0;
		} else {
			PokemonMove* move = new PokemonMove(PokemonType::T_TYPELESS, getPower(), 1.0, 1);
			damage = mech->calculateDamage(move, attacker, defender);
			delete move;
		}

		teamInfo_s team(defender->getMaster()->getBattleTeamIdx(), defender->getMaster()->getBattleTeamPosition());
		attacker->getField()->applyEffect(new DelayedDamageEffect(damage, team, 3));
		return 0;
	}
};

#endif