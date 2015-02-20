#ifndef INC_MOVESR_H_
#define INC_MOVESR_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "multiplehitmove.h"
#include "healthbasedmove.h"
#include "highcriticalhitmove.h"
#include "pokemonmove.h"
#include "statuseffect.h"
#include "damagelistenermove.h"
#include "fixedattackmove.h"

#include "pokemon.h"
#include "statchangeeffect.h"
#include "flincheffect.h"
#include "jewelmechanics.h"
#include "percenteffect.h"
#include "raineffect.h"
#include "weathermove.h"
#include "movelistentry.h"
#include "chargeeffect.h"
#include "intrinsicability.h"
#include "leechseedeffect.h"
#include "restrainingeffect.h"
#include "spikeseffect.h"
#include "statcuteffect.h"
#include "ingraineffect.h"
#include "confuseeffect.h"
#include "rollouteffect.h"

class Pokemon;
class BattleMechanics;

struct Rage : public StatusMove
{
	Rage()
	{
		statusEffectList_t a1;
		a1.push_back(new RageEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);
		StatusMove(PokemonType::T_NORMAL, 20, 1.0, 20, a1, a2, a3);
	}

private:
	class RageEffect : public StatusEffect
	{
	public:
		RageEffect() {};

		std::string getName() { return "Rage"; };
		std::string getDescription() { return "went into a rage!"; };

		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };
		void executeTurn(Pokemon *p, BattleTurn *turn) { p->removeStatus(this); };
		bool isListener() { return true; };
		void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) {
			std::string message = boost::str(boost::format("%s is enraged!") % target->getName());
			target->getField()->showMessage(message);
			target->addStatus(target, new StatChangeEffect(S_ATTACK, true));
		};
	};
};

struct RainDance : public WeatherMove<RainEffect>
{
	RainDance() : WeatherMove(PokemonType::T_WATER, 5, "Damp Rock") {};
};

struct RapidSpin : public PokemonMove
{
	RapidSpin() : PokemonMove(PokemonType::T_NORMAL, 20, 1.0, 40) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(getEffectiveness(attacker, defender) == 0.0) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			attacker->getField()->showMessage(message);
			
			return 0;
		} 

		if(attacker->hasEffect<RestrainingEffect>() || attacker->hasEffect<LeechSeedEffect>()) {
			attacker->removeStatus<RestrainingEffect>();
			attacker->removeStatus<LeechSeedEffect>();

			std::string message = boost::str(boost::format("%s was released") % attacker->getName());
			attacker->getField()->showMessage(message);
		}

		fieldEffects_t spikes = attacker->getField()->getEffectsByType<SpikesEffect>();
		bool blewAway = false;

		fieldEffects_t::iterator it = spikes.begin();
		for(; it != spikes.end(); ++it) {
			SpikesEffect* eff = dynamic_cast<SpikesEffect*>((*it));
			if(eff != NULL || eff->getLayers(attacker) >= 1) {
				eff->removeSpikes(attacker);
				blewAway = true;
			}
		}

		if(blewAway) {
			std::string message = boost::str(boost::format("%s blew away the spikes!") % attacker->getName());
			attacker->getField()->showMessage(message);
		}

		return PokemonMove::use(mech, attacker, defender);
	};
};

struct RazorLeaf : public HighCriticalHitMove
{
	RazorLeaf() : HighCriticalHitMove(PokemonType::T_GRASS, 55, 0.95, 25) {};
};

struct RazorWind : public StatusMove
{
	RazorWind() {
		statusEffectList_t a1;
		a1.push_back(new ChargeEffect(1, "created a whirlwind!", new MoveListEntry("Razor Wind", new HighCriticalHitMove(PokemonType::T_FLYING, 80, 1.0, 10) ) ) );
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);
		StatusMove(PokemonType::T_FLYING, 0, 1.0, 10, a1, a2, a3);
	};

	bool isAttack() { return true; };
	bool isDamaging() { return true; };
};

struct Recover : public StatusMove
{
	Recover() {
		statusEffectList_t a1;
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 10, a1, a2, a3);
	};
};

struct Reflect : public PokemonMove
{
	Reflect() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		teamInfo_s team(attacker->getMaster()->getBattleTeamIdx(), attacker->getMaster()->getBattleTeamPosition());
		if(!attacker->getField()->applyEffect(new StatCutEffect(S_DEFENCE, team, "reflect"))) {
			attacker->getField()->showMessage("But it failed!");
		}

		return 0;
	}

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };
};

struct Refresh : public PokemonMove
{
	Refresh() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		attacker->removeStatus(SPECIAL_EFFECT_LOCK);
		return 0;
	}
};

struct Rest : public PokemonMove
{
	Rest() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(attacker->hasAbility("Insomnia") 
			|| attacker->hasAbility("Vital Spirit") 
			|| attacker->hasEffect<SleepEffect>()
			|| (attacker->getHealth() == attacker->getStat(S_HP))) 
		{
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		attacker->removeStatus(SPECIAL_EFFECT_LOCK);

		//The turn it is applied still counts as a turn
		attacker->addStatus(attacker, new SleepEffect(3));
		int change = attacker->getStat(S_HP) - attacker->getHealth();
		attacker->changeHealth(change);

		return 0;
	}
};

struct Revenge : public DamageListenerMove
{
	Revenge() : DamageListenerMove(PokemonType::T_FIGHTING, 60, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		DamageListenerEffect* listener = getListener(attacker);
		if(listener == NULL) return 0;
		if(listener->getDamage() <= 0) {
			return DamageListenerMove::use(mech, attacker, defender);
		}

		int power = getPower();
		setPower(power * 2);
		int damage = DamageListenerMove::use(mech, attacker, defender);
		setPower(power);

		return damage;
	}

	int getPriority() { return -3; };
};

struct Reversal : public HealthBasedMove
{
	Reversal() : HealthBasedMove(PokemonType::T_FIGHTING, 1.0, 15, false) {};
};

struct Roar : public PokemonMove
{
	Roar() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 20) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasAbility("Suction Cups") || defender->hasEffect<IngrainEffect>()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		if(defender->isFainted())
			return 0;

		pokemonList_t party = defender->getTeammates();
		pokemonList_t::iterator it = party.begin();
		while(it != party.end())
		{
			Pokemon* p = (*it);
			if(p->isFainted() || (p == defender)) {
				it = party.erase(it);
			} else {
				++it;
			}
		}

		if(party.size() == 0) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		Pokemon* p = party.at(random_range(0,party.size() ) );
		p->getField()->switchInPokemon(p->getMaster(), p);
		p->addStatus(attacker, new RoarEffect());

		return 0;
	};

	int getPriority() { return -5; };

private:
	class RoarEffect : public StatusEffect
	{
	public:
		RoarEffect() {};

		std::string getName() { return ""; };
		std::string getDescription() { return ""; };

		int getTier() { return 1; };

		bool tick(Pokemon* p) {
			p->removeStatus(this);
			return true;
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };

		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) { return NULL; };
	};
};

struct RoarOfTime : public StatusMove
{
	RoarOfTime()
	{
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 150, 0.9, 5, a1, a2, a3);
	};
};

struct RockBlast : public MultipleHitMove
{
	RockBlast() : MultipleHitMove(PokemonType::T_ROCK, 25, 0.8, 10) {};
};

struct RockClimb : public StatusMove
{
	RockClimb() 
	{
		statusEffectList_t a1;
		a1.push_back(new ConfuseEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.2);

		StatusMove(PokemonType::T_NORMAL, 90, 0.85, 20, a1, a2, a3);
	}
};

struct RockPolish : public StatusMove
{
	RockPolish() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ROCK, 0, 1.0, 30, a1, a2, a3);
	};
};

struct RockSlide : public StatusMove
{
	RockSlide() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_ROCK, 75, 0.9, 10, a1, a2, a3);
	};
};

struct RockSmash : public StatusMove
{
	RockSmash() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_FIGHTING, 20, 1.0, 15, a1, a2, a3);
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		int power = (isAssignableFrom<JewelMechanics*>(mech)) ? 40 : 20;
		setPower(power);

		return StatusMove::use(mech, attacker, defender);
	}
};

struct RockTomb : public StatusMove
{
	RockTomb() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ROCK, 50, 0.8, 10, a1, a2, a3);
	};
};

struct RockWrecker : public StatusMove
{
	RockWrecker()
	{
		statusEffectList_t a1;
		a1.push_back(new RechargeEffect(1));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ROCK, 150, 0.9, 5, a1, a2, a3);
	};
};

struct RolePlay : public PokemonMove
{
	RolePlay() : PokemonMove(PokemonType::T_PSYCHIC, 0, 1.0, 10) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		IntrinsicAbility* abl = defender->getAbility();
		if(!abl->isSwappable()) {
			attacker->getField()->showMessage("But if failed!");
			return 0;
		}

		attacker->setAbility(abl, false);

		std::string message = boost::str(boost::format("%s copied its opponent's ability!") % attacker->getName());
		attacker->getField()->showMessage(message);

		return 0;
	}
};

struct Rollout : public FixedAttackMove 
{
	Rollout() : FixedAttackMove(PokemonType::T_ROCK, 30, 0.9, 20, new RolloutEffect("Rollout")) {};
};

struct Roost : public StatusMove
{
	Roost()
	{
		statusEffectList_t a1;
		a1.push_back(new RoostEffect());
		a1.push_back(new PercentEffect(0.5, false, -1, ""));
		boolArray_t a2(2, true);
		doubleArray_t a3(2, 1.0);

		StatusMove(PokemonType::T_ROCK, 150, 0.9, 5, a1, a2, a3);		
	};

private:
	class RoostEffect : public StatusEffect
	{
		pokemonTypeList_t m_types;

	public:
		RoostEffect() {};

		bool apply(Pokemon* p) {
			m_types = p->getTypes();

			pokemonTypeList_t types = m_types;
			pokemonTypeList_t::iterator it = types.begin();
			while(it != types.end()) {
				PokemonType* type = (*it);
				if(type->equals(PokemonType::T_FLYING)) {
					it = m_types.erase(it);
					continue;
				}

				++it;
			}

			p->setType(m_types);
			return true;
		}

		std::string getName() { return "Roosting"; };
		int getTier() { return 5; };
		std::string getDescription() { return ""; };
		void unapply(Pokemon* p) { p->setType(m_types); };
		bool tick(Pokemon* p) {
			p->removeStatus(this);
			return true;
		}
	};
};

#endif