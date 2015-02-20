#ifndef INC_TRAPPINGABILITY_H_
#define INC_TRAPPINGABILITY_H_

#include "definitions.h"
#include "opponenteffectability.h"
#include "trappingeffect.h"

class Pokemon;

class TrappingAbility : public OpponentEffectAbility
{
public:
	TrappingAbility(std::string name) : OpponentEffectAbility(name) {};
	~TrappingAbility() {};

	bool isTrappable(Pokemon* p) { return true; };

	void applyToOpponent(Pokemon* owner, Pokemon* p);
};

class TrappingEffectExt : public TrappingEffect
{
	TrappingAbility* m_friend;
public:
	TrappingEffectExt(TrappingAbility* frnd) : TrappingEffect(""), m_friend(frnd) {};
	~TrappingEffectExt() {};

	std::string getDescription() { return ""; };

	bool canSwitch(Pokemon* p);

	bool isPassable() { return false; };
};

#endif