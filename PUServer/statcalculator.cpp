#include <iostream>
#include <string>
#include <ctime>
#include <math.h>

#include "statcalculator.h"
#include "global.h"

int StatCalculator::randomNumber()
{
	static int lastSeed = 0;
	srand((unsigned int)(time(NULL) + lastSeed)); // Start rand();
	lastSeed = rand();

	return lastSeed;
}

int StatCalculator::Calculate_IV()
{
	return randomNumber() % 35 + 1;
}

int StatCalculator::Calculate_HP(int _base, int _iv, int _ev, int _level)
{
	return (((( 2 * _base ) + _iv) * _level / 100 ) + 10 + _level);
}

int StatCalculator::Calculate_Stats(int _base, int _iv, int _ev, int _level)
{
	return (((( 2 * _base ) + _iv) * _level / 100 ) + 5 );
}

int StatCalculator::Calculate_EXP(int _baseExp, int _level, bool isWild)
{
	double wild = (isWild) ? 1 : 1.5;
	double trade = 1; // If it's traded 1.5 (maby later)

	return (int)floor(((_baseExp * _level) * trade * wild) / 7);
}

int StatCalculator::Calculate_LevelEXP(int _level)
{
	return (int)floor(pow((double)_level, 3));
}

int StatCalculator::Calculate_WildEXP(int _level)
{
	int CurrentLvl = Calculate_LevelEXP(_level);
	int NextLvl = Calculate_LevelEXP(_level + 1);

	return randomNumber() % (NextLvl - CurrentLvl);
}

int StatCalculator::Calculate_HealTime(int _level, int _hpTotal, int _hpLeft)
{
	int healTime = _level;

	int PokemonHP = (int)floor(((double)_hpLeft / (double)_hpTotal) * 100.0);

	healTime = (int)floor(((double)_level / 100.) * PokemonHP);

	return (healTime * 500);
}

int StatCalculator::getTypeByName(std::string type)
{
	if(type == "ground")
		return 100;
	else if(type == "water")
		return 101;
	else if(type == "ghost")
		return 102;
	else if(type == "bug")
		return 103;
	else if(type == "fighting")
		return 104;
	else if(type == "psychic")
		return 105;
	else if(type == "grass")
		return 106;
	else if(type == "dark")
		return 107;
	else if(type == "normal")
		return 108;
	else if(type == "poison")
		return 109;
	else if(type == "electric")
		return 110;
	else if(type == "unknown")
		return 111;
	else if(type == "steel")
		return 112;
	else if(type == "rock")
		return 113;
	else if(type == "dragon")
		return 114;
	else if(type == "flying")
		return 115;
	else if(type == "fire")
		return 116;
	else if(type == "ice")
		return 117;
	else
		return 111; //unown
}

int StatCalculator::calculateAttackDamage(Pokemon *attacker, Pokemon *defender, int attackSlot, int attackReturn[], int turnID)
{
	int attackID = attacker->getAttack(attackSlot)->getAttackID();

	// Move info about this formula check here:
	// http://www.smogon.com/dp/articles/damage_formula
	// Note: everything has to be round DOWN
	int modifiers[4];
	statusModifiers(attacker->getAttack(attackSlot)->getEffectStatus(), attacker->getAttack(attackSlot)->getEffectChance(), attacker, defender, modifiers);

	// Check if move is special move
	bool attackerSpecialMove = (attacker->getAttack(attackSlot)->getAttackClass() == "special"); 
	
	// Get attack type
	int AttackType = StatCalculator::getTypeByName(attacker->getAttack(attackSlot)->getAttackType()); 

	// BasePower = HH × BP × IT × CHG × MS × WS × UA × FA
	double BasePower = calculateBasePower(attacker->getAttackDamage(attackSlot), attacker, attackSlot, defender, turnID);

	// SP_Atk = Stat × SM × AM × IM
	int AtkPower = (attackerSpecialMove) ? attacker->stat_sa : attacker->stat_at;
	int AtkModi = (attackerSpecialMove) ? attacker->mod_sa : attacker->mod_at;
	double SP_Atk = calculateSPAttack(AtkPower, AtkModi, attackID);

	// SP_Def = Stat × SM × Mod × SX
	int DefPower = (attackerSpecialMove) ? attacker->stat_sd : attacker->stat_de;
	int DefModi = (attackerSpecialMove) ? attacker->mod_sd : attacker->mod_de;
	double SP_Def = calculateSPDefence(DefPower, DefModi, attackID);

	// 3 if the move is a Critical Hit and the user has the Sniper ability, 2 if the move is a Critical Hit and the user's ability is not Sniper, and 1 otherwise.
	int CriticalHit = calculateCriticalHit(attacker, attackSlot);

	// Mod1 = BRN × RL × TVT × SR × FF
	double Mod1 = 1;

	/* 1.3 if the user is holding the item Life Orb.
     * 1, 1.1, 1.2, 1.3, ..., 2 if the user is holding the item Metronome and has used the same move once, twice, three times, four times, ... etc. consecutively.
     * 1.5 if the user is attacking with the move Me First.
	 * 1 otherwise.
	*/
	double Mod2 = 1;

	if(attacker->getAttack(attackSlot)->getAttackID() == 382)
		Mod2 = 1.5;

	// (Rand * 100) ÷ 255, rounded down, where Rand is a random whole number between 217 and 255 inclusive with uniform probability. This produces a whole number between 85 and 100 inclusive, but not with uniform probability. See Section 8 for more details.
	double rnd = ((StatCalculator::randomNumber() % (255 - 217) + 217) * 100) / 255;
	int randomNumber = (int)floor(rnd);

	if(attacker->getAttack(attackSlot)->getAttackID() == 255)
		randomNumber = 100;

	// 2 if the move is of the same type as that of the user and the user has the Adaptability ability, 1.5 if the move is of the same type as that of the user and the user's ability is not Adaptability, and 1 otherwise. 
	// This is known as Same Type Attack Bonus (hence STAB).
	int STAB = 1;

	// 2 if the move is super effective against the first type of the foe, 0.5 if the move is not very effective against the first type of the foe, 0 if the move type does not affect the first type of the foe, and 1 otherwise.
	bool hasFlag = (defender->hasFlag(StatusAilment_FORESIGHT) || defender->hasFlag(StatusAilment_ODOR_SLEUTH));
	int DefendType1 = StatCalculator::getTypeByName(defender->getType1());
	double Type1 = compareTypes(AttackType, DefendType1, hasFlag);

	// 2 if the move is super effective against the second type of the foe, 0.5 if the move is not very effective against the second type of the foe, 0 if the move type does not affect the type of the foe, and 1 otherwise (or if the foe has only one type).
	std::string DefendType2 = defender->getType2();
	double Type2 = 1;
	if(DefendType2 != "") Type2 = compareTypes(AttackType, StatCalculator::getTypeByName(DefendType2), hasFlag);

	// Mod3 = SRF × EB × TL × TRB
	int Mod3 = 1;

	// Set everything into array
	if((Type1 > 0 && Type2 > 0) && CriticalHit > 1 && attacker->getAttackDamage(attackSlot) > 0)
		attackReturn[0] = BONUS_CRITICALHIT;
	else if(Type1 == 0 || Type2 == 0)
		attackReturn[0] = BONUS_NOTEFFECTIVE;
	else if(Type1 == 2 || Type2 == 2)
		attackReturn[0] = BONUS_VERYEFFECTIVE;
	else if(Type1 == 0.5 || Type2 == 0.5)
		attackReturn[0] = BONUS_NOTVERYEFFECTIVE;
	else
		attackReturn[0] = BONUS_NONE;

	// Attacker
	attackReturn[1] = modifiers[0]; // attack type
	attackReturn[2] = modifiers[1]; // attack mod
	
	// Defender
	attackReturn[3] = modifiers[2]; // defense type
	attackReturn[4] = modifiers[3]; // defense mod

	double AttackDamage = 1.0;
	if(BasePower > 0)
	{
		if(SP_Def == 0) SP_Def = 1;

		// Damage Formula = (((((((Level * 2 / 5) + 2) * BasePower * [Sp]Atk / 50) / [Sp]Def) * Mod1) + 2) * CH * Mod2 * R / 100) * STAB * Type1 * Type2 * Mod3
		AttackDamage = ((((((( ( ( (attacker->getPokemonLvl() * 2) / 5) + 2) * BasePower * SP_Atk) / 50) / SP_Def) * Mod1) + 2) * CriticalHit * Mod2 * randomNumber) / 100) * STAB * Type1 * Type2 * Mod3;
		AttackDamage = floor(AttackDamage);

		if(AttackType == 110 && defender->hasFlag(StatusAilment_MUD_SPORT)) {
			AttackDamage = AttackDamage / 2.; // 50% less damage
		}
	}
	else {
		AttackDamage = 0.0;
	}

	int retn[2];
	multipleHitCheck(attacker->getAttack(attackSlot)->getAttackID(), AttackDamage, retn);
	AttackDamage = retn[0];
	attackReturn[5] = retn[1];

	return (int)AttackDamage;
}

int StatCalculator::multipleHitCheck(int attackID, double damage, int retn[])
{
	int hitAmount = 1;

	// DoubleSlap, Comet Punch, Fury Attack, Pin MIssile, Spike Cannon, Barrage, Fury Swipes, Tripple Kick, Bone Rush, Arm Thrust, Bullet Seed, Icicle Spear, Rock Blast
	if(attackID == 3 || attackID == 4 || attackID == 31 || attackID == 42 || attackID == 131 || attackID == 140 || attackID == 154 || attackID == 167 || attackID == 198 || attackID == 292 || attackID == 331 || attackID == 333 || attackID == 350)
	{
		int rnd = random_range(1,100);
		if(rnd < 13) // 4 or 5 times
		{
			int amount = random_range(1,100);
			if(amount <= 50) { hitAmount = 4; }
			else { hitAmount = 5; }
		}
		else if(rnd < 38) // 2 or 3 times
		{
			int amount = random_range(1,100);
			if(amount <= 50) { hitAmount = 2; }
			else { hitAmount = 3; }
		}
	}
	else if(attackID == 24 || attackID == 41 || attackID == 155 || attackID == 458) { // Double Kick, Twineedle, Bonemerang, Double Hit
		hitAmount = 2;
	}

	// Do it!
	if(hitAmount > 1) {
		damage *= hitAmount;
	}

	retn[0] = (int)damage;
	retn[1] = hitAmount;

	return hitAmount;
}

int StatCalculator::calculateCriticalHit(Pokemon* attacker, int attackSlot)
{
	int attackID = attacker->getAttack(attackSlot)->getAttackID();
	int PEI = 0, SL = 0, HCHR = 0, FE = 0;

	// No critical hit
	StatusList list = attacker->statusTurnList;
	StatusList::iterator findMe = list.find(381); // Lucky Chant
	if(findMe != list.end()) {
		return 1;
	}

	if(attackID == 2 || attackID == 75 || attackID == 152 || attackID == 163 || attackID == 177 || attackID == 238)
		HCHR = 1;

	if(attacker->getPokemonRealID() == 113 && attacker->getItemID() == 233)
		PEI = 1;

	if(attacker->getItemID() == 209) // Scope Lens
		SL = 1;

	if(attacker->getLastAttackID() == 116)
		FE = 1;

	int criticalHit = min( (1 + ( PEI * 2 ) + ( SL * 1 ) + ( HCHR * 3 ) + ( FE * 1 ) ) , 5 );
	double hitPercent = 0.0;

	// Karate Chop,		 Razor Wind,		Slam,			Razor Leaf,		Crabhammer,			Aeroblast,		  Cross Chop,		 Air Cutter,		Poison Tail,
	if((attackID == 2 || attackID == 13 || attackID == 21 || attackID == 75 || attackID == 152 || attackID == 177 || attackID == 283 || attackID == 314 || attackID == 342 ||
	//	Attack Order,	   Blaze Kick,			Night Shade,	  Shadow Claw,		 Psycho Cut,		Cross Poison,	 Stone Edge,		  Spacial Rand
		  attackID == 454 ||attackID == 299 || attackID == 400 || attackID == 421 || attackID == 427 || attackID == 440 || attackID == 444 || attackID == 460) 
		&& criticalHit != 5) 
	{
		criticalHit++;
	}
	else if(attackID == 116) { // Focus Energy
		if(criticalHit < 4) {
			criticalHit += 2;
		}
		else if(criticalHit != 5) {
			criticalHit++;
		}
	}

	switch(criticalHit)
	{
	case 1:
		hitPercent = 6.25;
		break;
	case 2:
		hitPercent = 12.5;
		break;
	case 3:
		hitPercent = 25;
		break;
	case 4:
		hitPercent = 33.2;
		break;
	case 5:
		hitPercent = 50;
		break;
	}

	int randomNumber = random_range(1,100);

	if(randomNumber <= hitPercent)
		return 2;
	else
		return 1;
}

double StatCalculator::calculateSPDefence(int attack, int modifier, int attackID)
{
	double attackMod = 1.0;

	switch(modifier)
	{
	case 6:
		attackMod = 4;
		break;
	case 5:
		attackMod = 3.5;
		break;
	case 4:
		attackMod = 3;
		break;
	case 3:
		attackMod = 2.5;
		break;
	case 2:
		attackMod = 2;
		break;
	case 1:
		attackMod = 1.5;
		break;
	case -1:
		attackMod = 0.6667;
		break;
	case -2:
		attackMod = 0.5;
		break;
	case -3:
		attackMod = 0.4;
		break;
	case -4:
		attackMod = 0.3333;
		break;
	case -5:
		attackMod = 0.2857;
		break;
	case -6:
		attackMod = 0.25;
		break;
	}

	double SX = (attackID == 120 || attackID == 153) ? 0.5 : 1;

	int totalDefence = (int)(attack * attackMod * SX);

	if(attackID == 153) { // Explosion
		totalDefence = totalDefence >> 1;
	}

	return totalDefence;
}

double StatCalculator::calculateSPAttack(int attack, int modifier, int attackID)
{
	double attackMod = 1.0;

	if(attackID == 451) 
	{
		int rnd = random_range(1,100);
		if(rnd <= 70) {
			modifier++;
		}
	}

	switch(modifier)
	{
	case 6:
		attackMod = 4;
		break;
	case 5:
		attackMod = 3.5;
		break;
	case 4:
		attackMod = 3;
		break;
	case 3:
		attackMod = 2.5;
		break;
	case 2:
		attackMod = 2;
		break;
	case 1:
		attackMod = 1.5;
		break;
	case -1:
		attackMod = 0.6667;
		break;
	case -2:
		attackMod = 0.5;
		break;
	case -3:
		attackMod = 0.4;
		break;
	case -4:
		attackMod = 0.3333;
		break;
	case -5:
		attackMod = 0.2857;
		break;
	case -6:
		attackMod = 0.25;
		break;
	}

	return (attack * attackMod);
}

double StatCalculator::calculateBasePower(int basePower, Pokemon *attacker, int attackSlot, Pokemon *defender, int turnID)
{
	// BasePower = HH × BP × IT × CHG × MS × WS × UA × FA
	
	// Helping hand
	int helpingHand = 1;

	// Base Power
	int attackBasePower = basePower;

	int attackID = attacker->getAttack(attackSlot)->getAttackID();
	if(attackID == 57) {
		if(defender->getLastAttackID() == 291) {
			attackBasePower = basePower << 1;
		}
	}
	else if(attackID == 89) // Earthquake
	{
		if(defender->getLastAttackID() == 91 && defender->getLastTurnID() == turnID) {
			attackBasePower = basePower << 1;
		}
	}	
	else if(attackID == 67 || attackID == 137) // Low Kick, Glare
	{
		int weight = defender->getWeight();
		if(weight < 10) {
			basePower = 20;
		}
		else if(weight < 25) {
			basePower = 40;
		}
		else if(weight < 50) {
			basePower = 60;
		}
		else if(weight < 100) {
			basePower = 80;
		}
		else if(weight < 200) {
			basePower = 100;
		}
		else {
			basePower = 120;
		}
		
		attackBasePower = basePower;
	}
	else if(attackID == 216) // Return
	{
		attackBasePower = (int)floor((attacker->getHappiness() << 1) / 5.);
	}
	else if(attackID == 217) // Present
	{
		int rnd = random_range(0,3);
		if(rnd == 0) {
			attacker->increaseHP(80);
			attackBasePower = 0;
		}
		else if(rnd == 1) {
			attackBasePower = 40;
		}
		else if(rnd == 2) {
			attackBasePower = 80;
		}
		else {
			attackBasePower = 120;
		}
	}
	else if(attackID == 218) // Frustration
	{
		//Base Power = 102—floor(happiness * 2 / 5) If this formula returns 0, the Base Power is changed to 1. Max BP = 102
		basePower = 102 - (int)floor(((double)attacker->getHappiness() * 2.) / 5.);
		attackBasePower = (basePower < 1) ? 1 : basePower;
	}
	else if(attackID == 222)
	{
		int random = random_range(1,100);
		int basePower = 0;
		if(random <= 5) {
			basePower = 10;
		}
		else if(random <= 15) {
			basePower = 30;
		}
		else if(random <= 35) {
			basePower = 50;
		}
		else if(random <= 65) {
			basePower = 70;
		}
		else if(random <= 85) {
			basePower = 90;
		}
		else if(random <= 95) {
			basePower = 110;
		}
		else {
			basePower = 150;
		}

		attackBasePower = basePower;
	}
	else if(attackID == 255) // Spit Up
	{
		attackBasePower = 100;
	}
	else if(attackID == 372) // Assurance [100 if the foe was hurt that turn, 50 otherwise.]
	{ 
		attackBasePower = 50;

		std::map<int, TurnMoveInfo> attackList = defender->moveHistory;
		for (std::map<int, TurnMoveInfo>::iterator it = attackList.begin(); it != attackList.end(); ++it) 
		{
			TurnMoveInfo tmp = it->second;
			if(tmp.turnID  == turnID && tmp.hit && tmp.damage > 0) 
			{
				attackBasePower = 100;
			}
		}
	}
	else if(attackID == 419 || attackID == 279) // Avalanche && Revenge
												// [120 if the targeted Pokémon did damage to the user, 60 otherwise.]
	{
		bool didDamage = false;
		for (std::map<int, TurnMoveInfo>::iterator it = defender->moveHistory.begin(); it != defender->moveHistory.end(); ++it)		
		{
			TurnMoveInfo moveInfo = it->second;
			if(moveInfo.foe == attacker->getPokemonID() && moveInfo.hit == true)
			{
				didDamage = true;
				break;
			}
		}

		if(didDamage) {
			attackBasePower = 120;
		}
		else {
			attackBasePower = 60;
		}
	}
	else if(attackID == 362) // Brine [130 if the targeted Pokémon’s current HP is less than or equal to half of its maximum HP, rounded down, 65 otherwise.]
	{
		if(defender->getPokemonHP_Left() <= (defender->getPokemonHP_Total() / 2))
			attackBasePower = 130;
		else
			attackBasePower = 65;
	}
	else if(attackID == 462) { // Crush Grip ** [1 + (120 × Foe’s Current HP ÷ Foe’s Max HP), rounded down.]
		double tmp = 1 + (120 * (defender->getPokemonHP_Left() / defender->getPokemonHP_Total()));
		attackBasePower = (int)floor(tmp);
	}
	else if(attackID == 284) { // Eruption ** [150 × User’s Current HP ÷ User’s Max HP, rounded down.]
		double tmp = 150 * (attacker->getPokemonHP_Left() / attacker->getPokemonHP_Total());
		attackBasePower = (int)floor(tmp);
	}
	else if(attackID == 263) { // Facade [140 if the user is Paralyzed, Poisoned, or Burned, 70 otherwise.]
		if(attacker->hasFlag(StatusAilment_BURN) || attacker->hasFlag(StatusAilment_POISON) || 
			attacker->hasFlag(StatusAilment_POISON_BAD) || attacker->hasFlag(StatusAilment_PARALYSIS))
			attackBasePower = 140;
		else
			attackBasePower = 70;
	}
	else if(attackID == 175 || attackID == 179) { // Flail & Reversal
							   // [200 if CP is between 0 and 1, 150 if CP is between 2 and 5, 100 if CP is between 6 and 12, 
							   //		 80 if CP is between 13 and 21, 40 if CP is between 22 and 42, and 20 if CP is between 43 and 64, 
							   //		 where CP = User's Current HP × 64 ÷ User's Total HP, rounded down.]
		double hp =  floor(((double)attacker->getPokemonHP_Left() / (double)attacker->getPokemonHP_Total()) * 100);

		if(hp < 3.124) {
			attackBasePower = 200;
		}
		else if (hp < 9.374) {
			attackBasePower = 150;
		}
		else if (hp < 20.2124) {
			attackBasePower = 100;
		}
		else if (hp < 34.374) {
			attackBasePower = 80;
		}
		else if (hp < 67.1874) {
			attackBasePower = 40;
		}
		else {
			attackBasePower = 20;
		}
	}
	else if(attackID == 374) { // Fling **
		
	}
	else if(attackID == 218) { // Frustration [102 - (User's Happiness × 2 ÷ 5), rounded down. If BP is 0, it becomes 1.]
		double tmp = 102 - ((attacker->getHappiness() * 2) / 5);
		int bp = (int)floor(tmp);

		if(bp == 0)
			bp = 1;

		attackBasePower = bp;
	}
	else if(attackID == 210) { // Fury Cutter [10 if Fury Cutter was not already used or it missed when 
							   // it was last used, otherwise, BP is double the BP of the last used Fury Cutter. 
							   // If BP is greater than 160, it becomes 160.]
		int numofattacks = 0;
		bool lastHit = true;
		for (std::map<int, TurnMoveInfo>::iterator it = attacker->moveHistory.begin(); it != attacker->moveHistory.end(); ++it)		
		{
			lastHit = true;
			TurnMoveInfo moveInfo = (*it).second;
			if(moveInfo.attackID == 210)
			{
				if(moveInfo.hit == true)
					numofattacks++;
				else
					lastHit = false;
			}
		}	
		
		attackBasePower = 10;
		if(numofattacks > 0 && lastHit == true)
		{
			for(int i = 1; i <= numofattacks; i++)
			{
				attackBasePower *= 2;
			}
		}		
	}
	else if(attackID == 447 || attackID == 67) { /* Grass Knot & Low Kick
									[20 if W is between 0 and 10, 40 if W is between 10.1 and 25,
									60 if W is between 25.1 and 50, 80 if W is between 50.1 and 100, 100 if W is between 100.1 and 200, 
									and 120 otherwise, where W is the weight of the foe in kilograms.] */
		int Weight = defender->getWeight();
		if(Weight <= 10)
			attackBasePower = 20;
		else if(Weight <= 25)
			attackBasePower = 40;
		else if(Weight <= 50)
			attackBasePower = 60;
		else if(Weight <= 100)
			attackBasePower = 80;
		else if(Weight <= 200)
			attackBasePower = 100;
		else
			attackBasePower = 120;
	}
	else if(attackID == 441)
	{
		if(defender->getLastAttackID() == 19 || defender->getLastAttackID() == 340) {
			basePower = basePower * 2;
		}
	}
	else if(attackID == 360) { // Gyro Ball
		double tmp = 1 + (25 * (defender->iv_sp / attacker->iv_sp));
		if(tmp > 150.)
			attackBasePower = 150;
		else
			attackBasePower = (int)floor(tmp);
	}
	else if(attackID == 301 || attackID == 205) { // Ice Ball & Rollout
		attackBasePower = 30;

		bool defenseCurl = false;
		int CountHit = 0;
		for (std::map<int, TurnMoveInfo>::iterator it = attacker->moveHistory.begin(); it != attacker->moveHistory.end(); ++it)		
		{
			TurnMoveInfo moveInfo = it->second;
			if(moveInfo.attackID == 111) // Defense Curl
				defenseCurl = true;
			
			if(defenseCurl && moveInfo.attackID == attackID) {
				attackBasePower = 60;
				if(moveInfo.hit)
					CountHit++;
				else {
					attackBasePower = 30;
					CountHit = 0;
				}
			}
		}
		
		for(int i = 1; i <= CountHit; i++)
			attackBasePower *= 2;
	}
	else if(attackID == 237) { // Hidden Power - http://www.smogon.com/forums/showthread.php?t=20649
		// Emtpy for now
	}
	else if(attackID == 222) { // Magnitude
		int R = StatCalculator::randomNumber() % 99;

		if(R <= 4)
			attackBasePower = 10;
		else if (R <= 14)
			attackBasePower = 30;
		else if (R <= 34)
			attackBasePower = 50;
		else if (R <= 64)
			attackBasePower = 70;
		else if (R <= 84)
			attackBasePower = 90;
		else if (R <= 94)
			attackBasePower = 110;
		else
			attackBasePower = 150;
	}
	else if(attackID == 363) { // Natural Gift - Check moves page

	}
	else if(attackID == 267) { // Nature Power

	}
	else if(attackID == 371) { // Payback
		if(defender->moveHistory.size() <= 0)
			attackBasePower = 50;
		else
		{
			std::map<int, TurnMoveInfo>::iterator it = defender->moveHistory.end();
			--it;
			TurnMoveInfo moveInfo = it->second;

			if(moveInfo.turnID == turnID)
				attackBasePower = 100;
			else
				attackBasePower = 50;
		}
	}
	else if(attackID == 217) { // Present ***
		int R = StatCalculator::randomNumber() % 255;

		if (R <= 102)
			attackBasePower = 40;
		else if (R <= 179)
			attackBasePower = 80;
		else if (R <= 204)
			attackBasePower = 120;
	}
	else if(attackID == 386) { // Punishment [60 + (20 × Sum of positive stat modifiers of the foe). If BP is greater than 200, it becomes 200.]
		int totalPositiveModifiers = 0;
		
		if(defender->mod_ac > 0)
			totalPositiveModifiers += defender->mod_ac;
		if(defender->mod_at > 0)
			totalPositiveModifiers += defender->mod_at;
		if(defender->mod_de > 0)
			totalPositiveModifiers += defender->mod_de;
		if(defender->mod_sa > 0)
			totalPositiveModifiers += defender->mod_sa;
		if(defender->mod_sd > 0)
			totalPositiveModifiers += defender->mod_sd;
		if(defender->mod_sp > 0)
			totalPositiveModifiers += defender->mod_sp;

		attackBasePower = 60 + (20 * totalPositiveModifiers);
		if(attackBasePower > 200)
			attackBasePower = 200;
	}
	else if(attackID == 228) { // Pursuit
		attackBasePower = 40;
	}
	else if(attackID == 216) { // Return [User's Happiness × 2 ÷ 5, rounded down. If BP is 0, it becomes 1.]
		double tmp = (attacker->getHappiness() * 2) / 5;
		attackBasePower = (int)floor(tmp);

		if(attackBasePower == 0)
			attackBasePower = 1;
	}
	else if(attackID == 265) { // Smellingsalt [120 if the foe is paralyzed (and is healed from paralysis afterwards), 60 otherwise.]
		if(defender->hasFlag(StatusAilment_PARALYSIS)) {
			attackBasePower = 120;
			defender->removeFlag(StatusAilment_PARALYSIS);
		}
		else {
			attackBasePower = 60;
		}
	}
	else if(attackID == 255) { // Spit Up [100 if one Stockpile was used, 200 if two Stockpiles were used, 300 if three Stockpiles were used, 0 otherwise.]
		int StockpileCount = 0;
		for (std::map<int, TurnMoveInfo>::iterator it = attacker->moveHistory.begin(); it != attacker->moveHistory.end(); ++it)		
		{
			TurnMoveInfo moveInfo = it->second;
			if(moveInfo.attackID == 254 && moveInfo.hit) 
				StockpileCount++;
		}

		attackBasePower = 100 * StockpileCount;
	}
	else if(attackID == 23) { // Stomp [130 if the foe used the move Minimize, 65 otherwise.]
		if(defender->hasFlag(StatusAilment_MINIMIZE))
			attackBasePower = 130;
		else
			attackBasePower = 65;
	}
	else if(attackID == 167) { // Triple Kick
		int numKicks = 1;
		for (std::map<int, TurnMoveInfo>::iterator it = defender->moveHistory.begin(); it != defender->moveHistory.end(); ++it)		
		{
			TurnMoveInfo moveInfo = it->second;
			if(moveInfo.turnID == turnID && moveInfo.attackID == 167 && moveInfo.hit)
				numKicks++;
		}	

		attackBasePower = 10 * numKicks;
	}
	else if(attackID == 279) // Revenge
	{
		TurnMoveInfo info = defender->getLastTurnInfo();
		if(info.turnID == turnID && info.damage > 0 && info.attackID != 220) {
			attackBasePower = basePower << 1;
		}
	}
	else if(attackID == 376) { // Trump Card
		int usageLeft = attacker->getAttack(attackSlot)->getUsageLeft();
		if(usageLeft >= 4)
			attackBasePower = 40;
		else if(usageLeft == 3)
			attackBasePower = 50;
		else if(usageLeft == 2)
			attackBasePower = 60;
		else if(usageLeft == 1)
			attackBasePower = 80;
		else
			attackBasePower = 200;
	}
	else if(attackID == 358) { // Wake-up Slap [120 if the foe is asleep (and is healed from sleep afterwards), 60 otherwise.]
		if(defender->hasFlag(StatusAilment_SLEEP)) {
			attackBasePower = 120;
		}
		else {
			attackBasePower = 60;
		}
	}
	else if(attackID == 323) { // Water Spout ** [150 × User’s Current HP ÷ User’s Max HP, rounded down.]
		double tmp = 150 * attacker->getPokemonHP_Left() / attacker->getPokemonHP_Total();
		attackBasePower = (int)floor(tmp);
	}
	else if(attackID == 311) { // Weather Ball [100 if the weather is Sunny Day, Rain Dance, Sandstorm, Hail or Fog, 50 otherwise.]
		attackBasePower = 50;
	}
	else if(attackID == 378) { // Wring Out
		double tmp = 1 + (120 * defender->getPokemonHP_Left() / defender->getPokemonHP_Total());
		attackBasePower = (int)floor(tmp);
	}
	
	// Item Multiplier
	int ItemMultiplier = 1;

	// Charge
	int ChargePower = 1;
	if(attacker->getLastAttackID() == 268 && attacker->getAttack(attackSlot)->getAttackType() == "electric")
		ChargePower = 2;

	// Mud Sport
	int MudSport = 1;

	// Water Sport
	int WaterSport = 1;

	// user ability
	int UserAbility = 1;

	// Foe Ability
	int FoeAbility = 1;

	return (helpingHand * attackBasePower * ItemMultiplier * ChargePower * MudSport * WaterSport * UserAbility * FoeAbility);
}

double StatCalculator::compareTypes(int attackType, int defendType, bool hasForsight)
{
	if(attackType == 103) // Bug
	{
		if(defendType == 104 || defendType == 116 || defendType == 115 || defendType == 102 || defendType == 109 || defendType == 112)
			return 0.5;
		else if(defendType == 107 || defendType == 106 || defendType == 105)
			return 2;
	}
	else if(attackType == 107) // Dark
	{
		if(defendType == 107 || defendType == 104 || defendType == 112)
			return 0.5;
		else if(defendType == 102 || defendType == 105)
			return 2;
	}
	else if(attackType == 114) // Dragon
	{
		if(defendType == 112)
			return 0.5;
		else if(defendType == 114)
			return 2;
	}
	else if(attackType == 110) // Electric
	{
		if(defendType == 114 || defendType == 110 || defendType == 106)
			return 0.5;
		else if(defendType == 115 || defendType == 110)
			return 2;
		else if(defendType == 110)
			return 0;
	}
	else if(attackType == 104) // Fighting
	{
		if(defendType == 103 || defendType == 115 || defendType == 109 || defendType == 105)
			return 0.5;
		else if(defendType == 107 || defendType == 117 || defendType == 108 || defendType == 113 || defendType == 112)
			return 2;
		else if(defendType == 102 && !hasForsight)
			return 0;
	}
	else if(attackType == 116) // Fire
	{
		if(defendType == 114 || defendType == 116 || defendType == 113 || defendType == 101)
			return 0.5;
		else if(defendType == 103 || defendType == 106 || defendType == 117 || defendType == 112)
			return 2;
	}
	else if(attackType == 104) // Flying
	{
		if(defendType == 110 || defendType == 112 || defendType == 113)
			return 0.5;
		else if(defendType == 103 || defendType == 104 || defendType == 106)
			return 2;
	}
	else if(attackType == 102) // Ghost
	{
		if(defendType == 114 || defendType == 112)
			return 0.5;
		else if(defendType == 102 || defendType == 105)
			return 2;
		else if(defendType == 108)
			return 0;
	}
	else if(attackType == 106) // Grass
	{
		if(defendType == 103 || defendType == 114 || defendType == 116 || defendType == 115 || defendType == 106 || defendType == 109 || defendType == 112)
			return 0.5;
		else if(defendType == 100 || defendType == 113 || defendType == 101)
			return 2;
	}
	else if(attackType == 100) // Ground
	{
		if(defendType == 103 || defendType == 106)
			return 0.5;
		else if(defendType == 110 || defendType == 116 || defendType == 109 || defendType == 112 || defendType == 113)
			return 2;
		else if(defendType == 115)
			return 0;
	}
	else if(attackType == 117) // Ice
	{
		if(defendType == 116 || defendType == 117 || defendType == 112 || defendType == 101)
			return 0.5;
		else if(defendType == 114 || defendType == 115 || defendType == 106 || defendType == 100)
			return 2;
	}
	else if(attackType == 108) // Normal
	{
		if(defendType == 112 || defendType == 113)
			return 0.5;
		else if(defendType == 102 && !hasForsight)
			return 0;
	}
	else if(attackType == 109) // Poison
	{
		if(defendType == 102 || defendType == 100 || defendType == 109 || defendType == 113)
			return 0.5;
		else if(defendType == 106)
			return 2;
		else if(defendType == 112)
			return 0;
	}
	else if(attackType == 105) // Psychic
	{
		if(defendType == 105 || defendType == 112)
			return 0.5;
		else if(defendType == 109)
			return 2;
		else if(defendType == 107)
			return 0;
	}
	else if(attackType == 113) // Rock
	{
		if(defendType == 104 || defendType == 100 || defendType == 112)
			return 0.5;
		else if(defendType == 103 || defendType == 116 || defendType == 115 || defendType == 117)
			return 2;
	}
	else if(attackType == 112) // Steel
	{
		if(defendType == 110 || defendType == 116 || defendType == 112 || defendType == 101)
			return 0.5;
		else if(defendType == 117 || defendType == 113)
			return 2;
	}
	else if(attackType == 101) // Water
	{
		if(defendType == 114 || defendType == 106 || defendType == 101)
			return 0.5;
		else if(defendType == 116 || defendType == 100 || defendType == 113)
			return 2;
	}

	return 1;
}

void StatCalculator::statusModifiers(string attackStatus, int effectChance, Pokemon *attacker, Pokemon *defender, int returnModifiers[])
{
	vector<string> modifiers = Global::StringSplit(attackStatus, " ");

	Stat attackType = STAT_NONE;
	StatModifier attackMod = STATMOD_NONE;
	Stat defenseType = STAT_NONE;
	StatModifier defenseMod = STATMOD_NONE;
	
	returnModifiers[0] = STAT_NONE;
	returnModifiers[1] = STATMOD_NONE;
	returnModifiers[2] = STAT_NONE;
	returnModifiers[3] = STATMOD_NONE;

	int percent = random_range(1, 100);
	
	if(effectChance > 0 && effectChance < percent)
		return;

	for(std::vector<string>::iterator it = modifiers.begin(); it != modifiers.end(); it++)
	{
		string myStatus = it->c_str();

		// Attack
		if(myStatus == "ac-") {
			defender->setModAc(-1); defenseType = STAT_ACCURACY; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "at+"){
			attacker->setModAt(1); attackType = STAT_ATTACK; attackMod = STATMOD_ROSE;
		}
		else if(myStatus == "at++"){
			attacker->setModAt(2); attackType = STAT_ATTACK; attackMod = STATMOD_SHARPLYROSE;
		}
		else if(myStatus == "at-"){
			defender->setModAt(-1); defenseType = STAT_ATTACK; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "at--"){
			defender->setModAt(-2); defenseType = STAT_ATTACK; defenseMod = STATMOD_HARSHLYFELL;
		}
		// Defense
		else if(myStatus == "de+"){
			attacker->setModDe(1); attackType = STAT_DEFENSE; attackMod = STATMOD_ROSE;
		}
		else if(myStatus == "de++"){
			attacker->setModDe(2); attackType = STAT_DEFENSE; attackMod = STATMOD_SHARPLYROSE;
		}
		else if(myStatus == "de-"){
			defender->setModDe(-1); defenseType = STAT_DEFENSE; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "de--"){
			defender->setModDe(-2); defenseType = STAT_DEFENSE; defenseMod = STATMOD_HARSHLYFELL;
		}
		// Special attack
		else if(myStatus == "sa+"){
			attacker->setModSa(1); attackType = STAT_SPECIALATTACK; attackMod = STATMOD_ROSE;
		}
		else if(myStatus == "sa++"){
			attacker->setModSa(2); attackType = STAT_SPECIALATTACK; attackMod = STATMOD_SHARPLYROSE;
		}
		else if(myStatus == "sa-"){
			defender->setModSa(1); defenseType = STAT_SPECIALATTACK; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "sa--"){
			defender->setModSa(2); defenseType = STAT_SPECIALATTACK; defenseMod = STATMOD_HARSHLYFELL;
		}
		// Special defence
		else if(myStatus == "sd+"){
			attacker->setModSd(1); attackType = STAT_SPECIALDEFENSE; attackMod = STATMOD_ROSE;
		}
		else if(myStatus == "sd++"){
			attacker->setModSd(2); attackType = STAT_SPECIALDEFENSE; attackMod = STATMOD_SHARPLYROSE;
		}
		else if(myStatus == "sd-"){
			defender->setModSd(1); defenseType = STAT_SPECIALDEFENSE; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "sd--"){
			defender->setModSd(2); defenseType = STAT_SPECIALDEFENSE; defenseMod = STATMOD_HARSHLYFELL;
		}
		// Speed
		else if(myStatus == "sp+"){
			attacker->setModSp(1); attackType = STAT_SPEED; attackMod = STATMOD_ROSE;
		}
		else if(myStatus == "sp++"){
			attacker->setModSp(2); attackType = STAT_SPEED; attackMod = STATMOD_SHARPLYROSE;
		}
		else if(myStatus == "sp-"){
			defender->setModSp(1); defenseType = STAT_SPEED; defenseMod = STATMOD_FELL;
		}
		else if(myStatus == "sp--"){
			defender->setModSp(2); defenseType = STAT_SPEED; defenseMod = STATMOD_HARSHLYFELL;
		}

		//<!-- TODO:  at^ -->//

		else if(myStatus == "att"){
			defender->setFlag(StatusAilment_ATTRACTION); defenseType = STAT_ATTRACTION; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "brn"){
			defender->setFlag(StatusAilment_BURN); defenseType = STAT_BURN; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "cfz"){
			defender->setFlag(StatusAilment_CONFUSION); defenseType = STAT_CONFUSED; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "fln"){
			defender->setFlag(StatusAilment_FLINCH); defenseType = STAT_FLINCH; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "frz"){
			defender->setFlag(StatusAilment_FREEZE); defenseType = STAT_FREEZE; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "lsd"){
			defender->setFlag(StatusAilment_LEECH_SEED); defenseType = STAT_LEECH; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "par"){
			defender->setFlag(StatusAilment_PARALYSIS); defenseType = STAT_PARALIZED; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "psn"){
			defender->setFlag(StatusAilment_POISON); defenseType = STAT_POISON; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "slp"){
			defender->setFlag(StatusAilment_SLEEP); defenseType = STAT_SLEEP; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "tar"){
			defender->setStatus(STAT_TARGETING); defenseType = STAT_TARGETING; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "ter"){
			defender->setStatus(STAT_TERRAIN); defenseType = STAT_TERRAIN; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "tox"){
			defender->setFlag(StatusAilment_POISON_BAD); defenseType = STAT_TOXIC; defenseMod = STATMOD_NONE;
		}
		else if(myStatus == "tri")
		{
			int r = random_range(1,3);
			if(r == 1) {
				defender->setFlag(StatusAilment_BURN); defenseType = STAT_BURN; defenseMod = STATMOD_NONE;
			}
			else if(r == 2) {
				defender->setFlag(StatusAilment_FREEZE); defenseType = STAT_FREEZE; defenseMod = STATMOD_NONE;
			}
			else if(r == 3) {
				defender->setFlag(StatusAilment_PARALYSIS); defenseType = STAT_PARALIZED; defenseMod = STATMOD_NONE;
			}
		}
		else if(myStatus == "trp"){
			defender->setStatus(STAT_TRAP); defenseType = STAT_TRAP; defenseMod = STATMOD_NONE;
		}
	}

	returnModifiers[0] = attackType;
	returnModifiers[1] = attackMod;
	returnModifiers[2] = defenseType;
	returnModifiers[3] = defenseMod;
}