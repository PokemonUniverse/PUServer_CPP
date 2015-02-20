#include "attack.h"

Attack::Attack(Database *db, int _attackID, int _uniqueID, int _attackUsed)
{
	std::stringstream attackQuery;
	attackQuery << "SELECT name, blurb_dp, type, power, pp, accuracy, class, combos, effect_chance, status, move_effect_id, target, contact"
				<< " FROM moves WHERE id='" << _attackID << "'";

	Recordset *rst = new Recordset(db, attackQuery.str().c_str());
	
	if(rst->moveNext())
	{
		attackID = _attackID;
		uniqueID = _uniqueID;
		attackName = rst->getString("name");
		attackDesc = rst->getString("blurb_dp");
		attackType = rst->getString("type");
		attackClass = rst->getString("class");
		attackPower = rst->getInt("power");
		attackAccuracy = rst->getInt("accuracy");
		attackCombos = rst->getString("combos");
		
		usageTotal = rst->getInt("pp");
		usageLeft = usageTotal - _attackUsed;

		effectID = rst->getInt("move_effect_id");
		effectStatus = rst->getString("status");
		effectChance = rst->getInt("effect_chance");

		target = rst->getString("target");
		makesContact = rst->getString("contact");
	}

	if(target == "enemyfield") {
		target = "enemy field";
	}
	else if(target == "randenemy") {
		target = "random enemy";
	}
	else if(target == "lastenemy") {
		target = "last enemy";
	}

	isDisabled = false;

	delete rst;
}