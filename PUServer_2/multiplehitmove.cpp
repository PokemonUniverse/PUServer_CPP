#include "multiplehitmove.h"

#include "pokemon.h"
#include "battlemechanics.h"

uint32_t MultipleHitMove::use( BattleMechanics *mech, Pokemon *attacker, Pokemon *defender )
{
	int hits = 1;
	
	if(attacker->hasAbility("Skill Link")) {
		hits = 5;
	}
	else
	{
		int rnd = random_range(0, 100);
		if(rnd <= 37) {
			hits = 2;
		} else if(rnd <= 75) {
			hits = 3;
		} else if(rnd <= 87) {
			hits = 4;
		} else {
			hits = 5;
		}
	}

	int damage = 0;
	for(int i = 0; i < hits; ++i) {
		int partial = mech->calculateDamage(this, attacker, defender);
		defender->changeHealth(-partial);
		damage += partial;
	}

	std::string message;
	if(hits > 1) {
		message = boost::str(boost::format("Hit %s times!") % hits);
	} else {
		message = "Hit 1 time!";
	}
	defender->getField()->showMessage(message);

	return damage;
}