#ifndef INC_MOVEST_H_
#define INC_MOVEST_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "recoilmove.h"
#include "rampagemove.h"

#include "statchangeeffect.h"
#include "flincheffect.h"
#include "burneffect.h"
#include "paralysiseffect.h"
#include "confuseeffect.h"
#include "toxiceffect.h"
#include "poisoneffect.h"
#include "speedswapeffect.h"
#include "spikeseffect.h"
#include "toxicspikeseffect.h"
#include "tailwindeffect.h"

#include <boost/array.hpp>

struct Tailwind : public PokemonMove
{
	Tailwind() : PokemonMove(PokemonType::T_FLYING, 0, 1.0, 30) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		teamInfo_s team(attacker->getMaster()->getBattleTeamIdx(), attacker->getMaster()->getBattleTeamPosition());
		attacker->getField()->applyEffect(new TailwindEffect(team));
		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct Taunt : public StatusMove
{
	Taunt() {
		statusEffectList_t a1;
		a1.push_back(new TauntEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 20, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

private:
	class TauntEffect : public StatusEffect
	{
		int m_turns;
	public:
		TauntEffect() {};

		bool apply(Pokemon* p) {
			m_turns = random_range(3,5);
			return true;
		};

		std::string getDescription() { return "fell for the taunt!"; };
		std::string getName() { return "Taunt"; };

		int getTier() { return 1; };

		bool tick(Pokemon* p) {
			if(--m_turns == 0) {
				p->removeStatus(this);
				std::string message = boost::str(boost::format("%s's taunt wore off!") % p->getName());
				p->getField()->showMessage(message);

				return true;
			}

			return false;
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };
		bool hitsThroughSubstitude() { return true; } // TODO: note: does not hits through in advance
		
		bool vetoesMove(Pokemon *p, MoveListEntry *entry) 
		{
			std::string name = entry->getName();
			if(name == "Struggle") {
				return false;
			}
			
			boost::array<std::string, 4> a = { "Nature Power", "Sleep Talk", "Assist", "Metronome" };
			for(int i = 0; i < 4; ++i) {
				if(a.at(i) == name) return true;
			}

			return !entry->getMove()->isDamaging();
		}

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move)
		{
			if(vetoesMove(p, move)) {
				p->getField()->informUseMove(p, move->getName());
				std::string message = boost::str(boost::format("%s can't use %s after this taunt!") % p->getName() % move->getName());
				p->getField()->showMessage(message);

				return NULL;
			}

			return move;
		}
	};
};

struct TailGlow : public StatusMove
{
	TailGlow() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPATTACK, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_BUG, 0, 1.0, 20, a1, a2, a3);
	};
};

struct TailWhip : public StatusMove
{
	TailWhip() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct TakeDown : public RecoilMove
{
	TakeDown() : RecoilMove(PokemonType::T_NORMAL, 90, 0.85, 20, 0.25) {};
};

struct TeeterDance : public StatusMove
{
	TeeterDance() {
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Teleport : public PokemonMove
{
	Teleport() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("But it failed!");
		return 0;
	}
};

struct Thief : public PokemonMove
{
	Thief() : PokemonMove(PokemonType::T_DARK, 40, 1.0, 40) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			return PokemonMove::use(mech, attacker, defender);
		}

		/* TODO: Create when HoldItem is implemented
		HoldItem enemyItem = target.getItem();                  
				if ((enemyItem != null) && (user.getItem() == null) && !(target.hasAbility("Sticky Hold"))) {
					user.setItem(enemyItem);
					target.setItem(null);
					user.getField().showMessage(user.getName() + " stole " + target.getName()
						+ "'s " + enemyItem.getName() + "!");
				} else*/
		if(defender->hasAbility("Sticky Hold")) {
			std::string message = boost::str(boost::format("%s held on with its Sticky Hold!") % defender->getName());
			attacker->getField()->showMessage(message);
		}

		return PokemonMove::use(mech, attacker, defender);
	};
};

struct Thrash : public RampageMove
{
	Thrash() : RampageMove(PokemonType::T_NORMAL, 90, 1.0, 20) {};
};

struct Thunder : public StatusMove
{
	Thunder() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_ELECTRIC, 120, 0.7, 10, a1, a2, a3);
	};
};

//TODO: Thunder Fang - Correct probabilities?
struct ThunderFang : public StatusMove
{
	ThunderFang() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 0.1);

		StatusMove(PokemonType::T_ELECTRIC, 65, 0.95, 15, a1, a2, a3);
	};
};

struct ThunderWave : public StatusMove
{
	ThunderWave() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ELECTRIC, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Thunderbolt : public StatusMove
{
	Thunderbolt() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ELECTRIC, 95, 1.0, 15, a1, a2, a3);
	};
};

struct Thunderpunch : public StatusMove
{
	Thunderpunch() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ELECTRIC, 75, 1.0, 15, a1, a2, a3);
	};
};

struct Thundershock : public StatusMove
{
	Thundershock() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ELECTRIC, 40, 1.0, 30, a1, a2, a3);
	};
};

struct Tickle : public StatusMove
{
	Tickle() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, false));
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 20, a1, a2, a3);
	};
};

struct Torment : public StatusMove
{
	Torment()
	{
		statusEffectList_t a1;
		a1.push_back(new TormentEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_DARK, 0, 1.0, 15, a1, a2, a3);
	};

private:
	class TormentEffect : public StatusEffect
	{
		MoveListEntry* m_entry;

	public:
		TormentEffect() { m_entry = NULL; };
		
		std::string getName() { return "Torment"; };
		std::string getDescription() { return "was subjected to Torment!"; };
		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };

		bool apply(Pokemon* p) {
			m_entry = p->getLastMove();
			return StatusEffect::apply(p);
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
			if(move->equals(m_entry)) {
				std::string message = boost::str(boost::format("%s couldn't use the move after the torment!") % p->getName());
				p->getField()->showMessage(message);

				return NULL;
			}

			m_entry = move;
			return move;
		};

		bool vetoesMove(Pokemon *p, MoveListEntry *entry) {
			if(m_entry == NULL) {
				return false;
			}

			return m_entry->equals(entry);
		};
	};
};

struct Toxic : public StatusMove 
{
	Toxic() {
		statusEffectList_t a1;
		a1.push_back(new ToxicEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_POISON, 0, 0.75, 35, a1, a2, a3);
	}
};

struct ToxicSpikes : public PokemonMove
{
	ToxicSpikes() : PokemonMove(PokemonType::T_POISON, 0, 1.0, 20) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		BattleField* field = attacker->getField();
		ToxicSpikesEffect* spikes = dynamic_cast<ToxicSpikesEffect*>(SpikesEffect::getSpikes<ToxicSpikesEffect>(field));
		if(spikes == NULL) {
			spikes = new ToxicSpikesEffect();
			field->applyEffect(spikes);
		}

		spikes->addSpikes(defender);
		return 0;
	}
};

struct TriAttack : public StatusMove
{
	TriAttack() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		a1.push_back(new BurnEffect());
		a1.push_back(new FreezeEffect());
		boolArray_t a2(3, false);
		doubleArray_t a3(3, 0.1);

		StatusMove(PokemonType::T_NORMAL, 80, 1.0, 10, a1, a2, a3);
	};
};

struct TrickRoom : public PokemonMove
{
	TrickRoom() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 5) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		BattleField* field = attacker->getField();
		FieldEffect* effect = field->getEffectByType<SpeedSwapEffect>();

		if(effect != NULL) {
			field->removeEffect(effect);
		}
		else {
			field->showMessage(boost::str(boost::format("%s twisted the dimensions!") % attacker->getName()));
			field->applyEffect(new SpeedSwapEffect());
		}

		return 0;
	};

	int getPriority() { return -5; };
};

struct TripleKick : public PokemonMove
{
	TripleKick() : PokemonMove(PokemonType::T_FIGHTING, 60, 0.9, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = 0;

		PokemonMove* move = new PokemonMove(PokemonType::T_FIGHTING, 0, 1.0, 1);
		for(int i = 1; i < 4; ++i) {
			move->setPower(i * 10);
			damage += attacker->useMove(move, defender);
		}
		delete move;

		attacker->getField()->showMessage("Hit 3 times!");
		return damage;
	};
};

struct Trick : public PokemonMove
{
	Trick() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		if(defender->hasAbility("Sticky Hold") || attacker->hasAbility("Sticky Hold"))
		{
			std::string message;
			if(attacker->hasAbility("Sticky Hold")) {
				message = boost::str(boost::format("%s hung on with its Sticky Hold") % attacker->getName());
				attacker->getField()->showMessage(message);
			}
			if(defender->hasAbility("Sticky Hold")) {
				message = boost::str(boost::format("%s hung on with its Sticky Hold") % defender->getName());
				attacker->getField()->showMessage(message);
			}

			return 0;
		}

/*TODO: Create when HoldItem is implemented
		HoldItem targetItem = target.getItem();
                    HoldItem item = user.getItem();                   
                    HoldItem userItem = (item == null) ? null : (HoldItem)item.clone();
                    user.setItem(targetItem);
                    target.setItem(userItem);
                    if (targetItem != null) {
                        user.getField().showMessage(user.getName() + " obtained " + targetItem.getName() + "!");
                    }
                    if (userItem != null) {
                        user.getField().showMessage(target.getName() + " obtained " + userItem.getName() + "!");
                    }
                    return 0;*/

		attacker->getField()->showMessage("But it failed!");
		return 0;
	};

	bool isAttack() { return true; };
	bool isDamaging() { return false; };
};

struct TrumpCard : public PokemonMove
{
	TrumpCard() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int pp = 5;
		for(int i = 0; i < 4; ++i)
		{
			MoveListEntry* entry = attacker->getMove(i);
			if(entry == NULL) continue;
			if(entry->getName() == "Trump Card") {
				pp = attacker->getPp(i);
				break;;
			}
		}

		if(pp > 4) { 
			setPower(35);
		} else if (pp == 4) {
			setPower(50);
		} else if (pp == 3) {
			setPower(60);
		} else if (pp == 2) {
			setPower(75);
		} else {
			setPower(190);
		}

		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(0);

		return damage;
	};

	bool isAttack() { return true; };
};

struct Twineedle : public StatusMove
{
	Twineedle() {
		statusEffectList_t a1;
		a1.push_back(new PoisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_BUG, 25, 1.0, 15, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = 0;
		for(int i = 0; i < 2; ++i) {
			damage += StatusMove::use(mech, attacker, defender);
		}

		return damage;
	}
};

struct Twister : public StatusMove
{
	Twister() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_DRAGON, 40, 1.0, 20, a1, a2, a3);
	};
};

#endif