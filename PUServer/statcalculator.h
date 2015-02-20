#ifndef __b_calculator_h_
#define __b_calculator_h_

#include "battle.h"
#include "pokemon.h"
#include "attack.h"

class Pokemon;

using namespace std;

class StatCalculator
{
public:
	static int randomNumber();

	static int Calculate_IV();
	static int Calculate_HP(int _base, int _iv, int _ev, int _level);
	static int Calculate_Stats(int _base, int _iv, int _ev, int _level);
	static int Calculate_EXP(int _base, int _level, bool isWild);
	static int Calculate_LevelEXP(int _level);
	static int Calculate_WildEXP(int _level);

	static int Calculate_HealTime(int _level, int _hpTotal, int _hpLeft);

	static int getTypeByName(std::string typeName);

	int calculateAttackDamage(Pokemon* attacker, Pokemon* defender, int attackSlot, int attackReturn[], int turnID);

private:
	int calculateCriticalHit(Pokemon* attacker, int attackSlot);
	double calculateSPAttack(int attack, int modifier, int attackID);
	double calculateSPDefence(int attack, int modifier, int attackID);
	double calculateBasePower(int basePower, Pokemon* attacker, int attackSlot, Pokemon* defender, int turnID);

	double compareTypes(int attackType, int DefendType, bool hasForsight = false);
	void statusModifiers(string attackStatus, int effectChance, Pokemon* attacker, Pokemon* defender, int returnModifiers[]);
	int multipleHitCheck(int attackID, double damage, int retn[]);
};

#endif