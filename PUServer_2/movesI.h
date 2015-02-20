#ifndef INC_MOVESI_H_
#define INC_MOVESI_H_

#include "definitions.h"
#include "statusmove.h"
#include "pokemontype.h"
#include "multiplehitmove.h"
#include "fixedattackmove.h"

#include "statchangeeffect.h"
#include "freezeeffect.h"
#include "ingraineffect.h"
#include "paralysiseffect.h"
#include "rollouteffect.h"

struct IceBall : public FixedAttackMove 
{
	IceBall() : FixedAttackMove(PokemonType::T_ICE, 30, 0.9, 20, new RolloutEffect("Ice Ball")) {};
};

struct IceBeam : public StatusMove
{
	IceBeam() {
		statusEffectList_t a1;
		a1.push_back(new FreezeEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ICE, 95, 1.0, 10, a1, a2, a3);
	};
};

struct IceFang : public StatusMove
{
	IceFang() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		a1.push_back(new FreezeEffect());
		boolArray_t a2(2, false);
		doubleArray_t a3(2, 0.1);

		StatusMove(PokemonType::T_ICE, 65, 0.95, 15, a1, a2, a3);
	};
};

struct IcePunch : public StatusMove
{
	IcePunch() {
		statusEffectList_t a1;
		a1.push_back(new FreezeEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.1);

		StatusMove(PokemonType::T_ICE, 75, 1.0, 15, a1, a2, a3);
	};
};

struct IceShard : public PriorityMove
{
	IceShard() : PriorityMove(PokemonType::T_ICE, 40, 1.0, 30, 1) {};
};

struct IcicleSpear : public MultipleHitMove
{
	IcicleSpear() : MultipleHitMove(PokemonType::T_ICE, 10, 1.0, 30) {};
};

struct IcyWind : public StatusMove
{
	IcyWind() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_SPEED, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_ICE, 55, 0.95, 15, a1, a2, a3);
	};
};

struct Imprison : public StatusMove
{
	Imprison() {
		statusEffectList_t a1;
		a1.push_back(new ImprisonEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_PSYCHIC, 0, 1.0, 15, a1, a2, a3);
	};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) { return true; };

private:
	class ImprisonEffect : public StatusEffect
	{
	public:
		ImprisonEffect() {};

		std::string getName() { return "Imprison"; };
		std::string getDescription() { return "'s moves were sealed"; };

		int getTier() { return -1; };
		bool tick(Pokemon* p) { return false; };
		bool vetoesMove(Pokemon *p, MoveListEntry *entry) {
			Pokemon* target = p->getOpponent();
			for(int i = 0; i < 4; ++i) {
				MoveListEntry* move = target->getMove(i);
				if(move != NULL && move->equals(entry)) {
					return true;
				}
			}
			return false;
		};

		bool hitsThroughSubstitude() { return true; };
	};
};

struct Ingrain : public StatusMove
{
	Ingrain() {
		statusEffectList_t a1;
		a1.push_back(new IngrainEffect());
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_GRASS, 0, 1.0, 20, a1, a2, a3);
	};
};

struct IronDefense : public StatusMove
{
	IronDefense() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, true, 2));
		boolArray_t a2(1, true);
		doubleArray_t a3(1, 1.0);

		StatusMove(PokemonType::T_STEEL, 0, 1.0, 15, a1, a2, a3);
	};
};

struct IronHead : public StatusMove
{
	IronHead() {
		statusEffectList_t a1;
		a1.push_back(new FlinchEffect());
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_STEEL, 80, 1.0, 15, a1, a2, a3);
	};
};

struct IronTail : public StatusMove
{
	IronTail() {
		statusEffectList_t a1;
		a1.push_back(new StatChangeEffect(S_DEFENCE, false));
		boolArray_t a2(1, false);
		doubleArray_t a3(1, 0.3);

		StatusMove(PokemonType::T_ICE, 100, 0.75, 15, a1, a2, a3);
	};
};
#endif