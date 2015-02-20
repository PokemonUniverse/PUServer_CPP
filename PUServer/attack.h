#ifndef __attack_h_
#define __attack_h_

#include <iostream>
#include <sstream>
#include "database.h"

class Attack
{
public:
	Attack(Database* db, int _attackID, int _uniqueID, int _attackUsed);
	~Attack() {};

	int getAttackID() { return attackID; }
	int getUniqueID() { return uniqueID; }
	int getAttackAccuracy() { return attackAccuracy; }
	int getAttackPower() { return attackPower; }
	std::string getAttackName() { return attackName; }
	std::string getAttackDesc() { return attackDesc; }
	std::string getAttackType() { return attackType; }
	std::string getAttackClass() { return attackClass; }

	int getUsageLeft() { return usageLeft; }
	void setUsageLeft(int left) { 
		usageLeft = left; 
		if(usageLeft < 0) { 
			usageLeft = 0;
		} 
		else if(usageLeft > usageTotal) {
			usageLeft = usageTotal;
		}
	}
	int getUsageTotal() { return usageTotal; }	
	void useAttack() { usageLeft--; }
	
	int getEffectID() { return effectID; }
	int getEffectChance() { return effectChance; }
	std::string getEffectStatus() { return effectStatus; } // brn, slp, frz, etc..	

	std::string getTarget() { return target; }
	std::string getContact() { return makesContact; }

	void resetAttack() { usageLeft = usageTotal; }
	void setDisable(bool yesno) { isDisabled = yesno; }
	bool getDisable() { return isDisabled; }

private:
	int uniqueID, attackID, attackCount, attackPower, attackAccuracy, usageLeft, usageTotal, effectChance, effectID;
	std::string attackName, attackDesc, attackType, attackClass, attackCombos, effectStatus;
	std::string target, makesContact;
	bool isDisabled;
};

#endif