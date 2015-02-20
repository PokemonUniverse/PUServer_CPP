#ifndef INC_MOVESE_H_
#define INC_MOVESE_H_

#include "definitions.h"
#include "statusmove.h"
#include "healthbasedmove.h"
#include "prioritymove.h"
#include "protectmove.h"
#include "delegationeffect.h"
#include "pokemonmove.h"

#include "pokemon.h"
#include "battlemechanics.h"
#include "pokemontype.h"
#include "burneffect.h"
#include "flincheffect.h"
#include "statmultiplier.h"
#include "statuseffect.h"
#include "protecteffect.h"
#include "statchangeeffect.h"

struct EarthPower : public StatusMove
{
	EarthPower() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_GROUND, 90, 1.0, 10, a1, a2, a3);
	}
};

struct Embargo : public PokemonMove
{
	Embargo() : PokemonMove(PokemonType::T_DARK, 0, 1.0, 15) {};
	
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(defender->hasSubstitute()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		defender->addStatus(attacker, new EmbargoEffect());
		return 0;
	};

private:
	class EmbargoEffect : public StatusEffect
	{
	public:
		EmbargoEffect() {};

		std::string getName() { return "Embargo"; };

		bool apply(Pokemon* p) {
			/*
			HoldItem* item = p->getItem();
			if(item != NULL && item->isActive()) {
				item->unapply(p);
				item->deactivate();
			}
			*/

			return true;
		}

		std::string getDescription() {
			return "'s item was nullified.";
		}

		void unapply(Pokemon* p) {
			/*
			HoldItem* item = p->getItem();
			if(item != NULL && item->isActive()) {
				item->activate();
				item->apply(p);
			}
			*/
		}
	};
};

struct Ember : public StatusMove
{
	Ember() {
		statusEffectList_t a1;
		a1.push_back(new BurnEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_FIRE, 40, 1.0, 25, a1, a2, a3);
	};
};

struct Encore : public StatusMove
{
	Encore() {
		statusEffectList_t a1;
		a1.push_back(new EncoreEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_NORMAL, 0, 1.0, 5, a1, a2, a3);
	};

private:
	class EncoreEffect : public StatusEffect
	{
		int m_turns;
		bool m_transform;
		MoveListEntry* m_entry;

	public:
		EncoreEffect() : m_transform(false) {};

		std::string getName() { return "Encore"; };
		std::string getDescription() { return "got an encore!"; };
		int getTier() { return 5; };
		bool hitsThroughSubstitude() { return true; };

		bool apply(Pokemon* p) {
			MoveListEntry* entry = p->getLastMove();
			if(entry == NULL) {
				p->getField()->showMessage("But it failed!");
				return false;
			}

			m_entry = entry;
			m_turns = random_range(4, 8);
			return true;
		};

		bool tick(Pokemon* p) {
			if(--m_turns <= 0) {
				std::string message = boost::str(boost::format("%s's encore ended.") % p->getName());
				p->getField()->showMessage(message);
				p->removeStatus(this);

				return true;
			}

			return false;
		};

		bool isMoveTransformer(bool enemy) { return !enemy; };
		MoveListEntry* getTransformedMove(Pokemon *p, MoveListEntry *move) {
			if(m_transform) {
				m_transform = false;
				return m_entry;
			}

			return move;
		};

		bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return !entry->equals(m_entry); };
	};
};

struct Endeavor : public PokemonMove
{
	Endeavor() : PokemonMove(PokemonType::T_NORMAL, 0, 1.0, 5) {};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender)
	{
		if(getEffectiveness(attacker, defender) == 0.0) {
			std::string message = boost::str(boost::format("It doesn't affect %s...") % defender->getName());
			attacker->getField()->showMessage(message);
			return 0;
		}

		if(attacker->getHealth() >= defender->getHealth()) {
			attacker->getField()->showMessage("But it failed!");
			return 0;
		}

		int damage = defender->getHealth() - attacker->getHealth();
		defender->changeHealth(-damage);

		return damage;
	};
};

struct Endure : public ProtectMove
{
	Endure() : ProtectMove(PokemonType::T_NORMAL, 10, new EndureEffect()) {};

private:
	struct EndureEffect : public DelegationEffect
	{
		EndureEffect() {};

		std::string getDescription() { return "became ready to endure!"; };
		bool isMoveTransformer(bool enemy) { return false; };
	};
};

struct EnergyBall : public StatusMove
{
	EnergyBall() 
	{
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPDEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_GRASS, 80, 1.0, 10, a1, a2, a3);
	}
};

struct Eruption : public HealthBasedMove
{
	Eruption() : HealthBasedMove(PokemonType::T_FIRE, 1.0, 5, true) {};
};

struct Explosion : public PokemonMove
{
	Explosion() : PokemonMove(PokemonType::T_NORMAL, 250, 1.0, 5) {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		return true;
	};

	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) 
	{
		if(defender->hasAbility("Damp")) {
			std::string message = boost::str(boost::format("%s's Damp prevents explosions!") % defender->getName());
			return 0;
		}

		int damage = 0;
		if(defender->hasEffect<ProtectEffect>()) {
			statusEffectList_t list = defender->getNormalStatuses(0);
			statusEffectList_t::iterator it = list.begin();
			for(; it != list.end(); ++it)
			{
				StatusEffect* status = (*it);
				if(isAssignableFrom<ProtectEffect*>(status)) {
					ProtectEffect* eff = dynamic_cast<ProtectEffect*>(status);
					std::string message = boost::str(boost::format("%s %s") % defender->getName() % eff->getDescription());
					attacker->getField()->showMessage(message);
					break;
				}
			}
		}
		else if(mech->attemptHit(this, attacker, defender))
		{
			StatMultiplier* mul = defender->getMultiplier(S_DEFENCE);
			double value = mul->getSecondMultiplier();
			mul->setSecondMultiplier(value / 2.0);
			damage = mech->calculateDamage(this, attacker, defender);
			mul->setSecondMultiplier(value);

			defender->changeHealth(-damage);
		}

		attacker->faint();
		return damage;
	};
};

struct Extrasensory : public StatusMove
{
	Extrasensory() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_PSYCHIC, 80, 1.0, 30, a1, a2, a3);
	};
};

struct Extremespeed : public PriorityMove
{
	Extremespeed() : PriorityMove(PokemonType::T_NORMAL, 80, 1.0, 5, 1) {};
};
#endif