#ifndef INC_OPPONENTEFFECTABILITY_H_
#define INC_OPPONENTEFFECTABILITY_H_

#include "definitions.h"
#include "intrinsicability.h"
#include "fieldeffect.h"

class Pokemon;
class BattleField;
class StatusEffect;

class OpponentEffectAbility : public IntrinsicAbility
{
public:
	OpponentEffectAbility(std::string name) : IntrinsicAbility(name) {};
	~OpponentEffectAbility() {};

	void unapply(Pokemon* p);

	bool switchOut(Pokemon* p);
	void switchIn(Pokemon *p);

	virtual void applyToOpponent(Pokemon* owner, Pokemon* p) = 0;

private:
	FieldEffect* m_effect;
};

class OpponentFieldEffect : public FieldEffect
{
public:
	OpponentFieldEffect(Pokemon* owner, OpponentEffectAbility* fr) : m_owner(owner), m_friend(fr) {};
	~OpponentFieldEffect() {};

	bool applyToField(BattleField* field) { return true; };
	void unapplyToField(BattleField* field) {};

	bool tickField(BattleField* field) { return false; };

	Pokemon* getOwner() { return m_owner; };

	bool equals(StatusEffect* eff);

	bool apply(Pokemon* p);

private:
	Pokemon* m_owner;
	OpponentEffectAbility* m_friend;
};
#endif