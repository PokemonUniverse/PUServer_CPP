#ifndef INC_MOVESM_H_
#define INC_MOVESM_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "perfectaccuracymove.h"
#include "prioritymove.h"
#include "typecutmove.h"
#include "countermove.h"
#include "pokemonmove.h"

#include "pokemon.h"
#include "battlemechanics.h"
#include "statchangeeffect.h"
#include "minimizeeffect.h"
#include "percenteffect.h"
#include "movelist.h"
#include "movelistentry.h"
#include "trappingeffect.h"
#include "restrainingeffect.h"
#include "mefirsteffect.h"
#include "magnetriseeffect.h"
#include "misteffect.h"

struct MachPunch : public PriorityMove
{
	MachPunch() : PriorityMove(PokemonType::T_FIGHTING, 40, 1.0, 30, 1) {};
};

struct MagicCoat : public PokemonMove
{
	MagicCoat() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 15) {};

	int getPriority() { return 5; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		std::string message = boost::str(boost::format("%1% created a barrier!") % attacker->getName());
		attacker->getField()->showMessage(message);

		defender->addStatus(attacker, new MagicCoatEffect());

		return 0;
	};

private:
	class MagicCoatEffect : public StatusEffect
	{
	public:
		MagicCoatEffect() {};

		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move)
		{
			PokemonMove* pmove = move->getMove();

			StatusMove* statusMove = dynamic_cast<StatusMove*>(pmove);
			if(statusMove == NULL) return move;
			if(!statusMove->isAttack() || statusMove->isDamaging()) return move;

			int max = statusMove->getEffects().size();
			for(int i = 0; i < max; ++i) {
				statusMove->setAttacker(i, true);
			}

			return move;
		};

		int getTier() { return 5; };

		bool tick(Pokemon* p) {
			p->removeStatus(this);
			return true;
		};
	};
};

struct MagicalLeaf : public PerfectAccuracyMove
{
	MagicalLeaf() : PerfectAccuracyMove(PokemonType::T_GRASS, 60, 20) {};
};

struct MagmaStorm : public StatusMove
{
	MagmaStorm() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Magma Storm", "trapped in a vortex"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIRE, 120, 0.7, 5, a1, a2, a3);
	};
};

struct MagnetBomb : public PerfectAccuracyMove
{
	MagnetBomb() : PerfectAccuracyMove(PokemonType::T_STEEL, 60, 20) {};
};

struct MagnetRise : public PokemonMove
{
	MagnetRise() : PokemonMove(PokemonType::T_ELECTRIC, 0, 0, 10) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		StatusEffect* eff = attacker->addStatus(attacker, new MagnetRiseEffect());
		if(eff == NULL) {
			attacker->getField()->showMessage("But it failed!");
		}

		return 0;
	}
};

struct Magnitude : public PokemonMove
{
	Magnitude() : PokemonMove(PokemonType::T_GROUND, 0, 1.0, 30) {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int chance = random_range(0, 100);
		int power = 0;
		int magnitude = 0;

		if(chance <= 5) {
			power = 10;
			magnitude = 4;
		} else if(chance <= 15) {
			power = 30;
			magnitude = 5;
		} else if(chance <= 35) {
			power = 50;
			magnitude = 6;
		} else if(chance <= 65) {
			power = 70;
			magnitude = 7;
		} else if(chance <= 85) {
			power = 90;
			magnitude = 8;
		} else if(chance <= 95) {
			power = 110;
			magnitude = 9;
		} else {
			power = 150;
			magnitude = 10;
		}

		std::string message = boost::str(boost::format("Magnitude %s!") % magnitude);
		setPower(power);
		int damage = mech->calculateDamage(this, attacker, defender);
		setPower(0);
		defender->changeHealth(-damage);

		return damage;
	};
};

struct MeFirst : public PokemonMove
{
	MeFirst() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source)
	{
		// Assume two pokemon
		/** Note: You cannot give PokemonMoves states
         * outside of their nature as a move - there is only one
         * copy for the whole program, not for each pokemon who
         * has the move! The latter would be a massive waste of
         * memory.
         */
		Pokemon* target = source->getOpponent();
		uint64_t oppIndex = target->getMaster()->getGUID();
		BattleTurn* opp = turn.at(oppIndex);
		if(opp == NULL || !opp->isMoveTurn()) return;

		MoveListEntry* entry = (MoveListEntry*)target->getMove(opp->getId())->clone();
		PokemonMove* move = entry->getMove();
		if(!move->isDamaging()) return;
		
		move->setPower((move->getPower() * 3) / 2);
		source->addStatus(source, new MeFirstEffect(entry));
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("But it failed!");
		return 0;
	};
};

struct MeanLook : public StatusMove
{
	MeanLook() {
		statusEffectList_t a1;
		a1.push_back(new TrappingEffect("Mean Look"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 5, a1, a2, a3);
	};
};

struct Meditate : public StatusMove
{
	Meditate() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 40, a1, a2, a3);
	};
};

struct MegaDrain : public AbsorbMove
{
	MegaDrain() : AbsorbMove(PokemonType::T_GRASS, 40, 1.0, 10, 0.5) {};
};

struct Memento : public StatusMove
{
	Memento() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false, 2));
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 10, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true; 
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(mech->attemptHit(this, attacker, defender)) {
			StatusMove::use(mech, attacker, defender);
		}

		attacker->faint();

		return 0;
	};
};

struct MetalBurst : public CounterMove
{
	MetalBurst() : CounterMove(PokemonType::T_STEEL, 1.0, 10, 3) {};

	int getPriority() { return 0; };
};

struct MetalClaw : public StatusMove
{
	MetalClaw() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_STEEL, 50, 1.0, 40, a1, a2, a3);
	};
};

struct MetalSound : public StatusMove
{
	MetalSound() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false, 2));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_STEEL, 0, 0.85, 40, a1, a2, a3);
	};
};

struct MeteorMash : public StatusMove
{
	MeteorMash() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_STEEL, 100, 0.85, 10, a1, a2, a3);
	};
};

struct Metronome : public PokemonMove
{
	Metronome() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		MoveListEntry* entry = NULL;
		do {
			entry = MoveList::getDefaultData()->getRandomMove();
			if(entry->getName() == "Focus Punch" || entry->getName() == "Metronome") {
				continue;
			}
		} while(false);

		return attacker->useMove(entry, defender);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true; 
	};
};

struct MilkDrink : public StatusMove
{
	MilkDrink() {
		statusEffectList_t a1;
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 10, a1, a2, a3);
	};
};

struct Mimic : public PokemonMove
{
	Mimic() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		MoveListEntry* move = defender->getLastMove();
		if(move == NULL) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		attacker->addStatus(attacker, new MimicEffect(move));
		return 0;
	}

private:
	class MimicEffect : public StatusEffect
	{
		MoveListEntry* m_entry;

	public:
		MimicEffect(MoveListEntry* entry) : m_entry(entry) {};

		std::string getName() { return boost::str(boost::format("Mimicking %s") % m_entry->getName()); };
		std::string getDescription() { return "mimicked the foe's move!"; };
		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
			if(move->getName() == "Mimic") {
				return m_entry;
			}

			return move;
		};
	};
};

struct Minimize : public StatusMove
{
	Minimize() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_EVASION, true));
		a1.push_back(new MinimizeEffect());
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct MiracleEye : public StatusMove
{
	MiracleEye()
	{
		statusEffectList_t a1;
		a1.push_back(new MiracleEyeEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 40, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		statusEffectList_t list = defender->getNormalStatuses(0);
		statusEffectList_t::iterator it = list.begin();
		for(; it != list.end(); ++it)
		{
			StatusEffect* effect = (*it);
			if(!isAssignableFrom<StatChangeEffect*>(effect)) continue;
			StatChangeEffect* eff = dynamic_cast<StatChangeEffect*>(effect);
			if(eff->getStat() == S_EVASION) {
				defender->removeStatus(eff);
			}
		}

		defender->getEvasion()->setSecondMultiplier(1);
		return StatusMove::use(mech, attacker, defender);
	};

private:
	class MiracleEyeEffect : public StatusEffect
	{
	public:
		MiracleEyeEffect() {};

		std::string getName() { return "Miracle"; };
		std::string getDescription() { return "indentified the enemy Pokemon"; };
		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };
		bool isEffectivenessTransformer(bool enemy) { return !enemy; };
		double getEnemyTransformedEffectiveness(PokemonType *move, PokemonType *pokemon) {
			if(move->equals(PokemonType::T_PSYCHIC) && pokemon->equals(PokemonType::T_DARK)) return 1.0;

			return StatusEffect::getEnemyTransformedEffectiveness(move, pokemon);
		};
	};
};

struct MirrorCoat : public CounterMove
{
	MirrorCoat() : CounterMove(PokemonType::T_PSYCHIC, 1.0, 20, 1) {};
};

struct MirrorMove : public PokemonMove
{
	MirrorMove() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(defender->getLastMove() == NULL) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		return attacker->useMove(defender->getLastMove(), defender);
	}
};

struct MirrorShot : public StatusMove
{
	MirrorShot() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_STEEL, 65, 0.85, 10, a1, a2, a3);
	}
};

struct Mist : public PokemonMove
{
	Mist() : PokemonMove(PokemonType::T_ICE, 0, 1.0, 30) {};
	
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		FieldEffect* eff = attacker->getField()->getEffectByType<MistEffect>();
		if(eff == NULL) {
			attacker->getField()->applyEffect(new MistEffect());
			eff = attacker->getField()->getEffectByType<MistEffect>();
		}

		MistEffect* effect = dynamic_cast<MistEffect*>(eff);
		if(effect != NULL) {
			effect->activateParty(attacker);
		}

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct MistBall : public StatusMove
{
	MistBall() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.5);

		StatusMove(PokemonType::T_PSYCHIC, 70, 1.0, 5, a1, a2, a3);
	};
};

struct MudBomb : public StatusMove
{
	MudBomb() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 65, 0.85, 10, a1, a2, a3);
	};
};

struct MudShot : public StatusMove
{
	MudShot() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 55, 0.95, 15, a1, a2, a3);
	};
};

struct MudSlap : public StatusMove
{
	MudSlap() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GROUND, 20, 1.0, 10, a1, a2, a3);
	};
};

struct MudSport : public TypeCutMove
{
	MudSport() : TypeCutMove(PokemonType::T_GROUND, 15, PokemonType::T_ELECTRIC, "Mud Sport") {};
};

struct MuddyWater : public StatusMove
{
	MuddyWater() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ACCURACY, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_WATER, 95, 0.85, 10, a1, a2, a3);
	};
};

#endif