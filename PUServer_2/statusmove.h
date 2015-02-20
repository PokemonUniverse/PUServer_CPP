#ifndef INC_STATUSMOVE_H_
#define INC_STATUSMOVE_H_

#include "definitions.h"
#include "pokemonmove.h"

class StatusEffect;
class PokemonType;

typedef std::vector<StatusEffect*> statusEffectList_t;

class StatusMove : public PokemonMove
{
public:
	StatusMove(PokemonType* type, int power, double accuracy, int pp, statusEffectList_t statuses, boolArray_t attacker, doubleArray_t chances)
		: PokemonMove(type, power, accuracy, pp)
	{
		m_bug = false;
		m_personal = true;

		m_statuses = statuses;
		m_chances = chances;
		m_attacker = attacker;

		for(int i = 0; i < attacker.size(); ++i) {
			if(!attacker[i]) {
				m_personal = false;
				break;
			}
		}

		if(power != 0) {
			m_personal = false;
		}
	};
	virtual ~StatusMove() {};

	bool attemptHit(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender) {
		if(m_personal)
			return true;

		return PokemonMove::attemptHit(mech, attacker, defender);
	};

	StatusMove* clone() { return new StatusMove(*this);	};

	statusEffectList_t getEffects() { return m_statuses; };
	bool isAttack() { return !m_personal; };
	bool isDamaging() { return (m_power != 0); };

	bool isBuggy() {
		return ( (m_statuses.size() != m_chances.size()) || (m_chances.size() != m_attacker.size()) );
	};

	bool getAttacker(int i) { return m_attacker.at(i); };
	void setAttacker(int idx, bool attacker) {
		if(idx < 0 || idx >= m_attacker.size()) return;
		m_attacker[idx] = attacker;
	}

	/**
     * Return whether this status effect can be successfully applied. This is
     * very random and not logical at all.
     */
	bool isEffective(Pokemon* target);

	/**
     * The rules for whether this move can hit are elaborate. If the move does
     * damage then the normal type rules apply; if the move has no effect then
     * it will not inflict statuses either. However, if the move does not
     * do damage and does not inflict one of the SPECIAL_EFFECT_LOCK statuses
     * then it can hit even types normally immune to the move's type.
     * <br /><br />
     * Two intrinsic abilities also affect this method. If the user has
     * Serene Grace then the chance of each effect being applied is double. If
     * the target has Shield Dust then none of the effects can be applied to it
     * so long as the move does damage.
     */
	uint32_t use(BattleMechanics *mech, Pokemon *attacker, Pokemon *defender);

protected:
	StatusMove() {};

private:
	statusEffectList_t m_statuses;
	doubleArray_t m_chances;
	boolArray_t m_attacker; // Does this status affect the attacker?
	bool m_bug;
	bool m_personal; // Is this a personal enhancement move?
};
#endif