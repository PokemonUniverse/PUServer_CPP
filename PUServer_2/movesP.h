#ifndef INC_MOVESP_H_
#define INC_MOVESP_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "multiplehitmove.h"
#include "pokemonmove.h"
#include "highcriticalhitmove.h"
#include "protecteffect.h"
#include "damagelistenermove.h"
#include "statchangeswapmove.h"
#include "rampagemove.h"

#include "pokemon.h"
#include "poisoneffect.h"
#include "statchangeeffect.h"
#include "confuseeffect.h"
#include "toxiceffect.h"
#include "freezeeffect.h"
#include "perishsongeffect.h"
#include "battleturn.h"
#include "damagelistenereffect.h"

struct PainSplit : public PokemonMove
{
	PainSplit() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		if(defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		int userHp = attacker->getHealth();
		int targetHp = defender->getHealth();
		int newHealth = (int)((userHp + targetHp) / 2);

		attacker->changeHealth((newHealth - userHp), true);
		defender->changeHealth((newHealth - targetHp), true);

		return 0;
	};
};

struct PayDay : public PokemonMove
{
	PayDay() : PokemonMove(PokemonType::T_NORMAL, 40, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("Coins scattered everywhere!");
		return PokemonMove::use(mech, attacker, defender);
	}
};

struct Payback : DamageListenerMove
{
	Payback() : DamageListenerMove(PokemonType::T_DARK, 50, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		DamageListenerEffect* listener = getListener(attacker);
		if(listener == NULL || listener->getDamage() <= 0) {
			return DamageListenerMove::use(mech, attacker, defender);
		}

		int power = getPower();
		setPower(power * 2);
		int damage = DamageListenerMove::use(mech, attacker, defender);
		setPower(power);

		return damage;
	};
};

struct PerishSong : public StatusMove
{
	PerishSong() {
		statusEffectList_t a1;
		a1.push_back(new PerishSongEffect(true));
		a1.push_back(new PerishSongEffect(true));
		boolArray_t a2;
		a2.push_back(true);
		a2.push_back(false);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 5, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("Both Pokemon will faint in 3 turns.");
		return StatusMove::use(mech, attacker, defender);
	}
};

struct PetalDance : public RampageMove
{
	PetalDance() : RampageMove(PokemonType::T_GRASS, 70, 1.0, 20) {};
};

struct PinMissile : public MultipleHitMove
{
	PinMissile() : MultipleHitMove(PokemonType::T_NORMAL, 18, 0.8, 15) {};
};

struct Pluck : public PokemonMove
{
	Pluck() : PokemonMove(PokemonType::T_FLYING, 60, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = getPower();
		/*
		HoldItem* item = defender->getItem();
		if(isAssignableFrom<Berry*>(item)) {
			setPower(power * 2);
		}*/


		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(power);

		return damage;
	}
};

struct PoisonFang : public StatusMove
{
	PoisonFang() {
		statusEffectList_t a1;
		a1.push_back(new ToxicEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_POISON, 50, 1.0, 15, a1, a2, a3);
	};
};

struct PoisonGas : public StatusMove
{
	PoisonGas() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_POISON, 0, 0.55, 40, a1, a2, a3);
	};
};

struct PoisonJab : public StatusMove
{
	PoisonJab() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_POISON, 80, 1.0, 20, a1, a2, a3);
	};
};

struct PoisonString : public StatusMove
{
	PoisonString() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_POISON, 15, 1.0, 35, a1, a2, a3);
	};
};

struct PoisonTail : public StatusMove
{
	PoisonTail() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_POISON, 50, 1.0, 25, a1, a2, a3);
	};

	bool hasHighCriticalHitRate() { return true; };
};

struct Poisonpowder : public StatusMove
{
	Poisonpowder() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_POISON, 0, 0.75, 35, a1, a2, a3);
	};
};

struct PowderSnow : public StatusMove
{
	PowderSnow() {
		statusEffectList_t a1;
		a1.push_back(new FreezeEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ICE, 40, 1.0, 25, a1, a2, a3);
	};
};

struct PowerSwap : public StatChangeSwapMove
{
	PowerSwap()
	{
		intArray_t list;
		list.push_back(S_ATTACK);
		list.push_back(S_SPATTACK);
		StatChangeSwapMove(PokemonType::T_PSYCHIC, 10, list);
	};
};

struct PowerTrick : public StatusMove
{
	PowerTrick()
	{
		statusEffectList_t a1;
		a1.push_back(new PowerTrickEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 10, a1, a2, a3);
	};

private:
	class PowerTrickEffect : public StatusEffect
	{
		int m_attack;
		int m_defence;

	public:
		PowerTrickEffect() {};

		std::string getName() { return "Power Trick"; };
		std::string getDescription() { return "swapped its stats!"; };
		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };

		bool apply(Pokemon* p) {
			m_attack = p->getRawStat(S_ATTACK);
			m_defence = p->getRawStat(S_DEFENCE);

			p->setRawStat(S_ATTACK, m_defence);
			p->setRawStat(S_DEFENCE, m_attack);

			return StatusEffect::apply(p);
		};

		void unapply(Pokemon* p) {
			p->setRawStat(S_ATTACK, m_attack);
			p->setRawStat(S_DEFENCE, m_defence);
		};
	};
};

struct Present : public PokemonMove
{
	Present() : PokemonMove(PokemonType::T_NORMAL, 0, 0.9, 15) {};

	bool isAttack() { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int rnd = random_range(0, 100);
		int power;

		if(rnd <= 40) {
			power = 40;
		} else if(rnd <= 70) {
			power = 80;
		} else if(rnd <= 80) {
			power = 120;
		} else {
			double maximum = (double)defender->getStat(S_HP);
			int restore = (int)(maximum * 0.2);

			defender->changeHealth(restore);
			return restore;
		}

		setPower(power);
		int damage = mech->calculateDamage(this, attacker, defender);
		setPower(0);

		defender->changeHealth(-damage);
		return damage;
	};
};

struct Protect : public ProtectMove
{
	Protect() : ProtectMove(PokemonType::T_NORMAL, 10, new myProtectEffect()) {};

private:
	class myProtectEffect : public ProtectEffect
	{
	public:
		myProtectEffect() {};
		std::string getDescription() { return " protected itself!"; };
	};
};


struct Psybeam : public StatusMove
{
	Psybeam() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_PSYCHIC, 65, 1.0, 20, a1, a2, a3);
	};
};

struct PsychUp : public PokemonMove
{
	PsychUp() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		attacker->removeStatus<StatChangeEffect>();

		statusEffectList_t statuses = defender->getNormalStatuses(0);
		statusEffectList_t::iterator it = statuses.begin();
		for(; it != statuses.end(); ++it) {
			StatusEffect* effect = (*it);
			if(isAssignableFrom<StatChangeEffect*>(effect))
			{
				StatChangeEffect* effectClone = (StatChangeEffect*)effect->clone();
				effectClone->setDescription("");
				// TODO: Open question: does Clear Body protect
				// against lowering stats by using this move?
				attacker->addStatus(defender, effectClone);
			}
		}

		std::string message = boost::str(boost::format("%s copied the opponent's stat changes!") % attacker->getName());
		attacker->getField()->showMessage(message);
		
		return 0;
	};
};

struct Psychic : public StatusMove
{
	Psychic() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_PSYCHIC, 90, 1.0, 10, a1, a2, a3);
	};
};

struct PsychoBoost : public StatusMove
{
	PsychoBoost() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, false, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 140, 0.9, 5, a1, a2, a3);
	};
};

struct PsychoCut : public HighCriticalHitMove
{
	PsychoCut() : HighCriticalHitMove(PokemonType::T_PSYCHIC, 70, 1.0, 20) {};
};

struct PsychoShift : public PokemonMove
{
	PsychoShift() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		StatusEffect* eff = attacker->getEffect(SPECIAL_EFFECT_LOCK);
		if(eff == NULL)
			return 0;

		if(!defender->hasEffect(SPECIAL_EFFECT_LOCK)) {
			StatusEffect* clone = eff->clone();
			attacker->removeStatus(eff);
			defender->addStatus(attacker, clone);
		}

		return 0;
	}
};

struct Psywave : public PokemonMove
{
	Psywave() : PokemonMove(PokemonType::T_NORMAL, 70, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int damage = attacker->getLevel() * ((random_range(0,10) + 50) / 100);
		defender->changeHealth(-damage);

		return damage;
	};

	bool isAttack() { return true; };
};

struct Punishment : public PokemonMove
{
	Punishment() : PokemonMove(PokemonType::T_DARK, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int raises = 0;
		statusEffectList_t statuses = defender->getNormalStatuses(0);
		statusEffectList_t::iterator it = statuses.begin();
		for(; it != statuses.end(); ++it)
		{
			StatusEffect* effect = (*it);
			if(!isAssignableFrom<StatChangeEffect*>(effect)) continue;

			StatChangeEffect* sEffect = dynamic_cast<StatChangeEffect*>(effect);
			if(sEffect->isRaise()) { raises++; }
		}

		setPower(60 + (20 * raises));
		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(0);

		return damage;
	};

	bool isAttack() { return true; };
};

struct Pursuit : public PokemonMove
{
	Pursuit() : PokemonMove(PokemonType::T_DARK, 40, 1.0, 20) {};

	void beginTurn(battleTurnList_t turn, int index, Pokemon *source)
	{
		Pokemon* target = source->getOpponent();
		uint32_t oppId = target->getMaster()->getGUID();
		BattleTurn* opp = turn.at(oppId);
		bool damageNow = false;

		if(opp == NULL)
			return;

		if(!opp->isMoveTurn()) {
			damageNow = true;
		}
		else {
			MoveListEntry* entry = target->getMove(opp->getId());
			if(entry->getName() == "U-turn") {
				if(target->getStat(S_SPEED) > source->getStat(S_SPEED)) {
					damageNow = true;
				}
			}
		}

		if(!damageNow)
			return;

		// Prevent this attack from occurring later in the turn
		turn.erase(oppId);

		if(source->isImmobilised(NULL))
			return;

		int power = getPower();
		setPower(power * 2);
		source->useMove(new MoveListEntry("Persuit", (PokemonMove*)clone()), target);
		setPower(power);
	}
};

#endif