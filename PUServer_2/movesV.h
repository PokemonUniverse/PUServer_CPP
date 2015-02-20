#ifndef INC_MOVESV_H_
#define INC_MOVESV_H_

#include "definitions.h"
#include "recoilmove.h"
#include "perfectaccuracymove.h"
#include "prioritymove.h"

struct VacuumWave : public PriorityMove
{
	VacuumWave() : PriorityMove(PokemonType::T_FIGHTING, 40, 1.0, 30, 1) {};
};

struct VitalThrow : public PerfectAccuracyMove
{
	VitalThrow() : PerfectAccuracyMove(PokemonType::T_FIGHTING, 70, 10) {};

	int getPriority() { return -1; };
};

struct VoltTackle : public RecoilMove
{
	VoltTackle() : RecoilMove(PokemonType::T_ELECTRIC, 120, 1.0, 15, 1.0/3.0) {};
};

#endif