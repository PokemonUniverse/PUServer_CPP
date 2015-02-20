#ifndef __INTRINSICABILITY_H_
#define __INTRINSICABILITY_H_

#include <vector>
#include <boost/unordered_map.hpp>

#include "definitions.h"

#include "statuseffect.h"
#include "movelistentry.h"

class IntrinsicAbility;
typedef boost::unordered_map<std::string, IntrinsicAbility*> IntrinsicAbilityMap_t;

class IntrinsicAbility : public StatusEffect
{
public:
	static void load();

	static IntrinsicAbility* getInstance(std::string ability) {
		IntrinsicAbilityMap_t::iterator it = m_map.find(ability);
		if(it == m_map.end()) {
			return NULL;
		}
		
		return it->second;
	};


	IntrinsicAbility(std::string _name) : m_name(_name) 
	{
		registerAbility();
	};

	IntrinsicAbility* clone() {	return new IntrinsicAbility(*this);	};

	StatusEffects_t getEffect() { return EFFECT_DEFAULT; };

	std::string getName() { return m_name; };

	bool isPassable() { return true; };

	// Rest of the virtual function from StatusEffect
	bool allowsStatus(StatusEffect* eff, Pokemon* source, Pokemon* target) {
		return true;
	};

	bool isExclusiveWith(StatusEffect* eff) {
		return (m_lock == NO_EFFECT_LOCK) ? false : (m_lock == eff->getLock());
	};

    bool immobilises(Pokemon* p) { return false; };

	std::string getDescription() { return ""; };

	bool tick(Pokemon *p) {	return false; };
	
	int getTier() {	return -1; };
	
	virtual void switchIn(Pokemon *p) { };
	bool switchOut(Pokemon* p) { return false; };
	
	void unapply(Pokemon* p) { };
	bool apply(Pokemon* p) { return true; };

	bool hitsThroughSubstitude() { return false; };

	bool isEffectivenessTransformer(bool enemy) { return false; }
  
	void executeTurn(Pokemon *p, BattleTurn *turn) { };
	
	virtual bool isMoveTransformer(bool enemy) { return false; };

	virtual bool isListener() { return false; };
	
	virtual void informDamaged(Pokemon *attacker, Pokemon *target, MoveListEntry *move, uint32_t damage) { };
	
	bool deactivates(Pokemon *p) { return false; };
	
	bool equals(IntrinsicAbility* obj) {
		return (obj->getName() == m_name);
	};

	bool isSingleton() { return true; };

	void informDuplicateEffect(Pokemon *p) { };

	bool canSwitch(Pokemon *p) { return true; };

	bool canSwitch() { return true; };

	bool vetoesMove(Pokemon *p, MoveListEntry *entry) { return false; };

	void beginTick() {};

	bool isEffectTransferrable() { return true; };

	bool isPhysical(MoveListEntry* entry) {
		return (std::find(m_physical.begin(), m_physical.end(), entry->getName()) != m_physical.end());
	};

	bool isSwappable() { return true; };
   
private:
	std::string m_name;

	static bool m_isLoaded;

	static std::vector<std::string> m_moldBreaker;
	static std::vector<std::string> m_physical;

protected:
	static IntrinsicAbilityMap_t m_map;

	IntrinsicAbility(bool _register, std::string _name) : m_name(_name)
	{
		if(_register) {
			registerAbility();
		}
	};

	void registerAbility()
	{
		m_map.insert(std::make_pair<std::string, IntrinsicAbility*>(m_name, this));
	};
};


#endif