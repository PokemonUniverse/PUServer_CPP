#ifndef INC_MOVESH_H_
#define INC_MOVESH_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "pokemonmove.h"
#include "jumpkickmove.h"
#include "onehitkillmove.h"
#include "recoilmove.h"
#include "statchangeswapmove.h"

#include "pokemon.h"
#include "jewelmechanics.h"
#include "statuseffect.h"
#include "burneffect.h"
#include "flincheffect.h"
#include "sleepeffect.h"
#include "haileffect.h"
#include "weathermove.h"
#include "statchangeeffect.h"
#include "rechargeeffect.h"
#include "percenteffect.h"

class Pokemon;
class BattleMechanics;

struct Hail : public WeatherMove<HailEffect>
{
	Hail() : WeatherMove(PokemonType::T_ICE, 5, "Icy Rock") {};
};

struct HammerArm : StatusMove
{
	HammerArm() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIGHTING, 100, 0.9, 10, a1, a2, a3);
	}
};

struct Harden : public StatusMove
{
	Harden() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 30, a1, a2, a3);
	};
};

struct Haze : public PokemonMove
{
	Haze() : PokemonMove(PokemonType::T_ICE, 0, 1.0, 30) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->removeStatus<StatChangeEffect>();
		defender->removeStatus<StatChangeEffect>();

		return 0;
	};
};

struct Headbutt : public StatusMove
{
	Headbutt() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_NORMAL, 70, 1.0, 15, a1, a2, a3);
	};
};

struct HeadSmash : public RecoilMove
{
	HeadSmash() : RecoilMove(PokemonType::T_ROCK, 150, 0.8, 5, 0.5) {};
};

struct HeartSwap : public StatChangeSwapMove
{
	HeartSwap()
	{
		intArray_t list;
		list.push_back(S_DEFENCE);
		list.push_back(S_SPDEFENCE);
		list.push_back(S_ATTACK);
		list.push_back(S_SPATTACK);
		list.push_back(S_SPEED);
		list.push_back(S_ACCURACY);
		list.push_back(S_EVASION);
		StatChangeSwapMove(PokemonType::T_PSYCHIC, 10, list);
	};
};

struct HeatWave : public StatusMove
{
	HeatWave() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIRE, 100, 0.3, 10, a1, a2, a3);
	};
};

struct HealBell : public PokemonMove
{
	HealBell() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		bool sound = (getMoveListEntry()->getName() == "Heal Bell");
		pokemonList_t teammates = attacker->getTeammates();
		pokemonList_t::iterator it = teammates.begin();
		for(; it != teammates.end(); ++it) {
			Pokemon* p = (*it);
			if(!p || p->isFainted() || (sound && p->hasAbility("Soundproof")))
				continue;

			p->removeStatus(SPECIAL_EFFECT_LOCK);
		}

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};
};

struct HealBlock : public PokemonMove
{
	HealBlock() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		defender->addStatus(attacker, new HealBlockEffect());
		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

private:
	class HealBlockEffect : public StatusEffect
	{
		int m_turns;

	public:
		HealBlockEffect() : m_turns(5) {};

		std::string getName() { return "Heal block"; };
		std::string getDescription() { return "was prevented from healing!'"; };

		bool isVetoed(MoveListEntry* entry)
		{
			std::string name = entry->getName();
			if(name == "Rest" || name == "Wish") return true;

			PokemonMove* move = entry->getMove();
			StatusMove* statusMove = dynamic_cast<StatusMove*>(move);
			if(statusMove == NULL) return false;
			
			statusEffectList_t effects = statusMove->getEffects();
			StatusEffect* eff = NULL;
			
			foreach(StatusEffect* it, effects)
			{
				PercentEffect* effect = dynamic_cast<PercentEffect*>(it);
				if(effect != NULL)
				{
					eff = it;
					if(effect->getTier() == -1 && effect->getPercent() > 0.0) {
						return true;
					}
				}
			}

			return false;
		};

		bool isMoveTransformer(bool enemy) { return enemy; };

		MoveListEntry* getEnemyTransformedMove(Pokemon *p, MoveListEntry *move)
		{
			if(isVetoed(move)) {
				std::string message = boost::str(boost::format("%1% can't use %2% becuase of Heal Block!") % p->getName() % move->getName());
				p->getField()->showMessage(message);

				return NULL;
			}

			return move;
		};

		int getTier() { return 3; } //TODO: FInd actual tier

		bool tick(Pokemon* p) {
			if(m_turns-- <= 0) {
				p->removeStatus(this);
				std::string message = boost::str(boost::format("%1%'s Heal Block wore off") % p->getName());
				return true;
			}

			return false;
		};

		bool vetoesMove(Pokemon *p, MoveListEntry *entry) {
			return isVetoed(entry);
		};

		void informDuplicateEffect(Pokemon *p) {
			std::string message = boost::str(boost::format("It filaed to affect %1%") % p->getName());
			p->getField()->showMessage(message);
		}
	};
};

struct HealOrder : public StatusMove
{
	HealOrder() {
		statusEffectList_t a1;
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_BUG, 0, 1.0, 10, a1, a2, a3);
	}
};

struct HealingWish : public PokemonMove
{
	HealingWish() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		BattleField* field = attacker->getField();
		if(field->getAliveCount(attacker->getMaster()) == 1) {
			field->showMessage("But it failed!");
			return 0;
		}
		
		attacker->faint();
		field->requestAndWaitForSwitch(attacker->getMaster());

		Pokemon* target = field->getActivePokemon(attacker->getMaster()->getBattleTeamIdx(), attacker->getMaster()->getBattleTeamPosition());
		target->changeHealth(target->getStat(S_HP));
		target->removeStatus(SPECIAL_EFFECT_LOCK);

		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

//TODO: Doens't do anything in 1 vs 1, need to be optimized for 2 vs 2
struct HelpingHand : public PokemonMove
{
	HelpingHand() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->getField()->showMessage("But it failed!");
		return 0;
	}

	int getPriority() { return 6; };
};

struct HiJumpKick : public JumpKickMove
{
	HiJumpKick() : JumpKickMove(PokemonType::T_FIGHTING, 85, 100, 0.90, 20) {};
};

struct HornDrill : public OneHitKillMove
{
	HornDrill() : OneHitKillMove(PokemonType::T_NORMAL, 5) {};
};

struct Howl : public StatusMove
{
	Howl() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 40, a1, a2, a3);
	};
};

struct HydroCannon : public StatusMove
{
	HydroCannon() {
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_WATER, 150, 0.9, 5, a1, a2, a3);
	};
};

struct HyperBeam : public StatusMove
{
	HyperBeam() {
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 150, 0.9, 5, a1, a2, a3);
	};
};

struct HyperFang : public StatusMove
{
	HyperFang() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_NORMAL, 80, 0.9, 15, a1, a2, a3);
	};
};

struct Hypnosis : public StatusMove
{
	Hypnosis() {
		statusEffectList_t a1;
		a1.push_back(new SleepEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 0.6, 20, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		double accuracy = (isAssignableFrom<JewelMechanics*>(mech)) ? 0.7 : 0.6;
		setAccuracy(getAccuracy() / 0.6 * accuracy);

		return StatusMove::attemptHit(mech, attacker, defender);
	}
};
#endif