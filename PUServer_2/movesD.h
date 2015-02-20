#ifndef INC_MOVESD_H_
#define INC_MOVESD_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemonmove.h"
#include "recoilmove.h"
#include "multiplehitmove.h"
#include "absorbmove.h"
#include "protectmove.h"
#include "protecteffect.h"

#include "pokemon.h"
#include "battlemechanics.h"
#include "jewelmechanics.h"

#include "pokemontype.h"
#include "statchangeeffect.h"
#include "confuseeffect.h"
#include "paralysiseffect.h"
#include "defensecurleffect.h"
#include "sleepeffect.h"
#include "invulnerablestateeffect.h"
#include "chargeeffect.h"
#include "delayeddamageeffect.h"

struct DarkPulse : public StatusMove
{
	DarkPulse() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_DARK, 80, 1.0, 15, a1, a2, a3);
	};
};

struct DarkVoid : public StatusMove
{
	DarkVoid() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 0.8, 10, a1, a2, a3);
	};
};

struct DefendOrder : public StatusMove
{
	DefendOrder() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, true));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_BUG, 0, 1.0, 10, a1, a2, a3);
	};
};

struct DefenseCurl : public StatusMove
{
	DefenseCurl() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		a1.push_back(new DefenseCurlEffect());
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 40, a1, a2, a3);
	};
};

struct Defog : public StatusMove
{
	Defog() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_EVASION, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 15, a1, a2, a3);
	};
};

struct DestinyBond : public StatusMove
{
	DestinyBond()
	{
		statusEffectList_t a1;
		a1.push_back(new DestinyBondEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GHOST, 0, 1.0, 5, a1, a2, a3);
	};

private:
	class DestinyBondEffect : public StatusEffect
	{
	public:
		DestinyBondEffect() {};

		std::string getName() { return "Destiny Bond"; };
		std::string getDescription() { return "is trying to take its foe with it!"; };
		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };
		void executeTurn(Pokemon *p, BattleTurn *turn) { p->removeStatus(this); };
		bool isListener() { return true; };
		bool hitsThroughSubstitude() { return true; };
		void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) {
			if(target->getHealth() <= 0) {
				std::string message = boost::str(boost::format("%s took %s with it!") % target->getName() % attacker->getName());
				target->getField()->showMessage(message);
				attacker->faint();
			}
		};
	};
};

struct Detect : public ProtectMove
{
	Detect() : ProtectMove(PokemonType::T_FIGHTING, 5, new myProtectEffect()) {};

private:
	struct myProtectEffect : public ProtectEffect
	{
		myProtectEffect() {};
		std::string getDescription() { return "braced itself!"; };
	};
};

struct Dig : public StatusMove
{
private:
	struct DigCharge : public PokemonMove
	{
		DigCharge() : PokemonMove(PokemonType::T_GROUND, 60, 1.0, 10) {};

		uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
			int power = (isAssignableFrom<JewelMechanics*>(mech)) ? 80 : 60;
			setPower(power);

			return PokemonMove::use(mech, attacker, defender);
		};
	};

public:
	Dig() {
		std::vector<std::string>list;
		list.push_back("Earthquake");
		list.push_back("Fissure");
		list.push_back("Magnitude");
		
		statusEffectList_t a1;
		a1.push_back(new InvulnerableStateEffect(list));
		a1.push_back(new ChargeEffect(1, "dug a hole!", new MoveListEntry("Dig", new DigCharge() ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_GROUND, 0, 1.0, 10, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct Disable : public StatusMove
{
	Disable() {
		statusEffectList_t a1;
		a1.push_back(new DisableEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 0.55, 20, a1, a2, a3);
	};

private:
	class DisableEffect : public StatusEffect
	{
		int m_turns;
		MoveListEntry* m_entry;

	public:
		DisableEffect() {};

		std::string getName() { return boost::str(boost::format("Disabled: %s") % m_entry->getName()); };
		std::string getDescription() { return "was disabled"; };

		int getTier() { return 5; };

		bool apply(Pokemon* p) {
			MoveListEntry* entry = p->getLastMove();
			if(entry == NULL) {
				p->getField()->showMessage("But it failed!");
				return false;
			}

			m_entry = entry;
			m_turns = random_range(4, 7);
			return true;
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move) {
			if(move->equals(m_entry)) {
				std::string message = boost::str(boost::format("%s's %s is disabled!") % p->getName() % move->getName());
				p->getField()->showMessage(message);
				return NULL;
			}

			return move;
		};

		bool tick(Pokemon* p) {
			if(--m_turns <= 0) {
				p->removeStatus(this);
				return true;
			}
			return false;
		};

		bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return entry->equals(m_entry); };
		bool hitsThroughSubstitude() { return true; };
	};
};

struct Discharge : public StatusMove
{
	Discharge() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_ELECTRIC, 80, 1.0, 15, a1, a2, a3);
	};
};

struct Dive : public StatusMove
{
	Dive() {
		std::vector<std::string> list(1, "Surf");
		statusEffectList_t a1;
		a1.push_back(new InvulnerableStateEffect(list));
		a1.push_back(new ChargeEffect(1, "dove underwater!", new MoveListEntry("Dive", new PokemonMove(PokemonType::T_WATER, 60, 1.0, 10) ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_WATER, 0, 1.0, 10, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct DizzyPunch : public StatusMove
{
	DizzyPunch() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_NORMAL, 70, 1.0, 10, a1, a2, a3);
	};
};

struct DoomDesire : public PokemonMove
{
	DoomDesire() : PokemonMove(PokemonType::T_STEEL, 120, 0.85, 5) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage;
		if(!PokemonMove::attemptHit(mech, attacker, defender)) {
			damage = 0;
		} else {
			PokemonMove* move = new PokemonMove(PokemonType::T_TYPELESS, getPower(), 1.0, 1);
			move->setMoveListEntry(getMoveListEntry());
			damage = mech->calculateDamage(move, attacker, defender);

			delete move;
		}

		teamInfo_s team(defender->getMaster()->getBattleTeamIdx(), defender->getMaster()->getBattleTeamPosition());
		attacker->getField()->applyEffect(new DelayedDamageEffect(damage, team, 3));

		return 0;
	}
};

struct DoubleEdge : public RecoilMove
{
	DoubleEdge() : RecoilMove(PokemonType::T_NORMAL, 120, 1.0, 20, 0.125) {};
};

struct DoubleHit : public PokemonMove
{
	DoubleHit() : PokemonMove(PokemonType::T_NORMAL, 35, 0.9, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage = 0;
		for(int i = 0; i < 2; ++i) {
			int partial = mech->calculateDamage(this, attacker, defender);
			defender->changeHealth(-partial);
			damage += partial;
		}

		attacker->getField()->showMessage("Hit 2 times!");
		return damage;
	};
};

struct DoubleKick : public PokemonMove
{
	DoubleKick() : PokemonMove(PokemonType::T_FIGHTING, 30, 1.0, 30) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = 0;
		for(int i = 0; i < 2; ++i) {
			int partial = mech->calculateDamage(this, attacker, defender);
			defender->changeHealth(-partial);
			damage += partial;
		}

		attacker->getField()->showMessage("Hit 2 times!");
		return damage;
	}
};
struct DoubleTeam : public StatusMove
{
	DoubleTeam() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_EVASION, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 15, a1, a2, a3);
	};
};

struct Doubleslap : public MultipleHitMove
{
	Doubleslap() : MultipleHitMove(PokemonType::T_NORMAL, 15, 0.85, 10) {};
};

struct DracoMeteor : public StatusMove
{
	DracoMeteor() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DRAGON, 140, 0.90, 5, a1, a2, a3);
	};
};

struct Dragonbreath : public StatusMove
{
	Dragonbreath() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_DRAGON, 60, 1.0, 20, a1, a2, a3);
	};
};

struct DragonDance : public StatusMove
{
	DragonDance() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		a1.push_back(new StatChangeEffect(S_SPEED, true));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_DRAGON, 0, 1.0, 20, a1, a2, a3);
	};
};

struct DragonRage : public PokemonMove
{
	DragonRage() : PokemonMove(PokemonType::T_DRAGON, 0, 1.0, 10) {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = 40;
		defender->changeHealth(-damage);
		return damage;
	};
};

struct DragonRush : public StatusMove
{
	DragonRush() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_DRAGON, 100, 0.75, 10, a1, a2, a3);
	};
};

struct DrainPunch : public AbsorbMove
{
	DrainPunch() : AbsorbMove(PokemonType::T_FIGHTING, 60, 1.0, 5, 0.5) {};
};

struct DreamEater : public AbsorbMove
{
	DreamEater() : AbsorbMove(PokemonType::T_PSYCHIC, 100, 1.0, 15, 0.5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(!defender->hasSubstitute() && defender->hasEffect<SleepEffect>()) {
			return AbsorbMove::use(mech, attacker, defender);
		}

		attacker->getField()->showMessage("But it failed!");
		return 0;
	}
};

struct Dynamicpunch : public StatusMove
{
	Dynamicpunch() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIGHTING, 100, 0.5, 5, a1, a2, a3);
	};
};

#endif