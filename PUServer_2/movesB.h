#ifndef INC_MOVESB_H_
#define INC_MOVESB_H_

#include "definitions.h"
#include "statusmove.h"
#include "multiplehitmove.h"
#include "recoilmove.h"
#include "prioritymove.h"

#include "pokemon.h"
#include "intrinsicability.h"
#include "battlemechanics.h"

#include "statchangeeffect.h"
#include "pokemontype.h"
#include "flincheffect.h"
#include "freezeeffect.h"
#include "paralysiseffect.h"
#include "invulnerablestateeffect.h"
#include "chargeeffect.h"
#include "restrainingeffect.h"
#include "trappingeffect.h"
#include "rechargeeffect.h"
#include "statcuteffect.h"
#include "leechseedeffect.h"
#include "perishsongeffect.h"
#include "coeffect.h"

struct Barrage : public MultipleHitMove
{
	Barrage() : MultipleHitMove(PokemonType::T_NORMAL, 15, 0.85, 20) {};
};

struct Barrier : public StatusMove
{
	Barrier() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 30, a1, a2, a3);
	};
};

struct BatonPass : public PokemonMove
{
	BatonPass() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 40) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		BattleField* field = attacker->getField();
		uint8_t party = attacker->getMaster()->getBattleTeamIdx();
		if(field->getAliveCount(attacker->getMaster()) == 1) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		statusEffectList_t list = attacker->getNormalStatuses(0);
		statusEffectList_t applied;

		statusEffectList_t::iterator it = list.begin();
		for(; it != list.end(); ++it)
		{
			StatusEffect* effect = (*it);
			if(effect->getLock() != SPECIAL_EFFECT_LOCK)
			{
				if(effect->isPassable())
				{
					if(isAssignableFrom<LeechSeedEffect*>(effect)) {
						applied.push_back(new LeechSeedEffect(false));
					}
					else if(isAssignableFrom<PerishSongEffect*>(effect)) {
						applied.push_back(new PerishSongEffect(false));
					}
					else {
						applied.push_back(effect->clone());
					}

					if(isAssignableFrom<CoEffect*>(effect)) {
						CoEffect* coeffect = dynamic_cast<CoEffect*>(effect);
						if( !coeffect->getType()->equals<AttractEffect>() ) {
							effect->disable();
						}
					}
				}
			}
		}

		int substitute = attacker->getSubstitute();
		field->requestAndWaitForSwitch(attacker);

		Pokemon* target = field->getActivePokemon(attacker->getMaster());
		target->setSubstitute(substitute);
		
		field->setNarrationEnabled(false);
		it = applied.begin();
		for(; it != applied.end(); ++it) {
			StatusEffect* effect = (*it);
			target->addStatus(target, effect);
		}
		field->setNarrationEnabled(true);
		
		return 0;
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct BeatUp : public PokemonMove
{
	BeatUp() : PokemonMove(PokemonType::T_NORMAL, 10, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		pokemonList_t team = attacker->getTeammates();
		int baseDefence = defender->getBase(S_DEFENCE);
		int power = getPower();
		int totalDamage = 0;

		pokemonList_t::iterator it = team.begin();
		for(; it != team.end(); ++it)
		{
			Pokemon* pokemon = (*it);
			if(pokemon->isFainted() || pokemon->hasEffect(SPECIAL_EFFECT_LOCK)) {
				continue;
			}

			int baseAttack = pokemon->getBase(S_ATTACK);
			bool stab = pokemon->isType(getType());
			int damage = (int)(((int)((int)(((int)((2 * pokemon->getLevel()) / 5.0 + 2.0)
				* baseAttack
				* power)
				/ baseDefence)
				/ 50.0)
				+ 2)
				* (stab ? 1.5 : 1.0));

			std::string message = boost::str(boost::format("%s's attack!"));
			attacker->getField()->showMessage(message);
			
			defender->changeHealth(-damage);
			totalDamage += damage;
		}

		return totalDamage;
	}
};

struct BellyDrum : public StatusMove
{
	BellyDrum() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true, 12));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 10, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int damage = (attacker->getStat(S_HP) / 2);
		if(attacker->getHealth() <= damage) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		IntrinsicAbility *ability = attacker->getAbility();
		if(attacker->hasAbility("Magic Guard")) {
			ability->deactivate();
		} else {
			ability = NULL;
		}

		attacker->changeHealth(-damage, true);
		if(ability != NULL) {
			ability->activate();
		}

		return StatusMove::use(mech, attacker, defender);
	}
};

struct Bide : public StatusMove
{
	Bide() {
		statusEffectList_t a1;
		a1.push_back(new BideEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 10, a1, a2, a3);
	};

	int getPriority() { return 1; };
	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

private:
	class BideEffect : public StatusEffect
	{
		int m_turns;
		int m_damage;

		class BideEffectMove : public PokemonMove
		{
			int p_damage;
		public:
			BideEffectMove(int damage) : PokemonMove(PokemonType::T_TYPELESS, 0, 1.0, 1), p_damage(damage) {};
			uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
				int change = 2 * p_damage;
				defender->changeHealth(-change);
				return change;
			}
		};

	public:
		BideEffect() : m_damage(0), m_turns(2) {};

		std::string getName() { return "Bide"; };
		std::string getDescription() { return "is storing energy!"; };

		int getTier() { return -1; };
		bool deactivates(Pokemon *p) { return true; };
		bool isListener() { return true; };
		void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) { m_damage += damage; };

		bool immobilises(Pokemon* p) {
			if(--m_turns <= 0) {
				std::string message = boost::str(boost::format("%s unleashes energy!") % p->getName());
				p->getField()->showMessage(message);

				p->useMove(new BideEffectMove(m_damage), p->getOpponent());
				p->removeStatus(this);
			}

			return true;
		}
	};
};

struct Bite : public StatusMove
{
	Bite() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_DARK, 60, 1.0, 25, a1, a2, a3);
	};
};

struct Bind : public StatusMove
{
	Bind() {
		statusEffectList_t a1;
		a1.push_back(new RestrainingEffect("Bind", "bound"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 15, 0.75, 20, a1, a2, a3);
	};
};

struct BlastBurn : public StatusMove
{
	BlastBurn() {
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_FIRE, 150, 0.9, 5, a1, a2, a3);
	};
};

struct Blizzard : public StatusMove
{
	Blizzard() {
		statusEffectList_t a1;
		a1.push_back(new FreezeEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ICE, 120, 0.7, 5, a1, a2, a3);
	};
};

struct Block : public StatusMove
{
	Block() {
		statusEffectList_t a1;
		a1.push_back(new TrappingEffect("Block"));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 5, a1, a2, a3);
	};
};

struct BodySlam : public StatusMove
{
	BodySlam() {
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_NORMAL, 85, 1.0, 15, a1, a2, a3);
	};
};

struct BoneClub : public StatusMove
{
	BoneClub() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_GROUND, 65, 0.85, 20, a1, a2, a3);
	};
};

struct BoneRush : public MultipleHitMove
{
	BoneRush() : MultipleHitMove(PokemonType::T_GROUND, 25, 0.8, 10) {};
};

struct Bonemerang : public PokemonMove
{
	Bonemerang() : PokemonMove(PokemonType::T_GROUND, 50, 0.9, 10) {};

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

struct BounceEffect : public StatusMove
{
	BounceEffect()
	{
		statusEffectList_t a1;
		a1.push_back(new ParalysisEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FLYING, 85, 0.85, 5, a1, a2, a3);
	};
};

struct Bounce : public StatusMove
{
	Bounce() {
		std::vector<std::string> list;
		statusEffectList_t a1;
		a1.push_back(new InvulnerableStateEffect(list));
		a1.push_back(new ChargeEffect(1, "bounced up!", new MoveListEntry("Bounce", new BounceEffect() ) ) );
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);
		
		StatusMove(PokemonType::T_FLYING, 0, 0.85, 5, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct BraveBird : public RecoilMove
{
	BraveBird() : RecoilMove(PokemonType::T_FLYING, 120, 1.0, 15, 1.0/3.0) {};
};

struct BrickBreak : public PokemonMove
{
	BrickBreak() : PokemonMove(PokemonType::T_FIGHTING, 75, 1.0, 15) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		teamInfo_s team(defender->getMaster()->getBattleTeamIdx(), defender->getMaster()->getBattleTeamPosition());
				
		fieldEffects_t effects = attacker->getField()->getEffectsByType<StatCutEffect>();
		fieldEffects_t::iterator it = effects.begin();
		for(; it != effects.end(); ++it) {
			StatCutEffect* eff = dynamic_cast<StatCutEffect*>((*it));
			if(eff->getParty() == team) {
				attacker->getField()->removeEffect(eff);
				attacker->getField()->showMessage("The wall shattered!");
			}
		}

		return PokemonMove::use(mech, attacker, defender);

		return 0;
	}

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct Brine : public PokemonMove
{
	Brine() : PokemonMove(PokemonType::T_WATER, 65, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		int power = getPower();
		if(defender->getHealth() < (defender->getStat(S_HP) / 2)) {
			setPower(power*2);
		}

		int damage = PokemonMove::use(mech, attacker, defender);
		setPower(power);

		return damage;
	}
};

struct Bubble : public StatusMove
{
	Bubble() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_WATER, 20, 1.0, 30, a1, a2, a3);
	};
};

struct Bubblebeam : public StatusMove
{
	Bubblebeam() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_WATER, 65, 1.0, 20, a1, a2, a3);
	};
};

struct BugBite : public PokemonMove
{
	BugBite() : PokemonMove(PokemonType::T_BUG, 60, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		/*
		HoldItem* item = defender->getItem();
		if(isAssignableFrom<Berry*>(item)) {
			Berry* berry = dynamic_cast<Berry*>(item);
			berry->executeEffects(attacker);
			defender->setItem(NULL);
		}*/

		return PokemonMove::use(mech, attacker, defender);

	}
};

struct BugBuzz : public StatusMove
{
	BugBuzz() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_BUG, 90, 1.0, 10, a1, a2, a3);
	};
};

struct BulkUp : public StatusMove
{
	BulkUp() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_ATTACK, true));
		a1.push_back(new StatChangeEffect(S_DEFENCE, true));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_FIGHTING, 0, 1.0, 20, a1, a2, a3);
	};
};

struct BulletPunch : public PriorityMove
{
	BulletPunch() : PriorityMove(PokemonType::T_STEEL, 40, 1.0, 30, 1) {};
};

struct BulletSeed : public MultipleHitMove
{
	BulletSeed() : MultipleHitMove(PokemonType::T_GRASS, 10, 1.0, 30) {};
};


#endif