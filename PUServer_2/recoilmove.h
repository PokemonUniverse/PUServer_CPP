#ifndef INC_RECOILMOVE_H_
#define INC_RECOILMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class Pokemon;
class PokemonType;
class BattleMechanics;

class RecoilMove : public PokemonMove
{
	double m_recoil;

public:
	RecoilMove(PokemonType* type, uint32_t power, double accuracy, uint32_t pp, double recoil)
		: PokemonMove(type, power, accuracy, pp), m_recoil(recoil) {};
	~RecoilMove() {};

	int getRecoil(Pokemon* p, int damage) {
		int recoil = (int)(m_recoil * (double)damage);
		return (recoil < 1) ? 1 : recoil;
	}

	bool isProtected(Pokemon* p);
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);
};
#endif