#ifndef INC_SPIKESEFFECT_H_
#define INC_SPIKESEFFECT_H_

#include "definitions.h"
#include "fieldeffect.h"

#include "pokemon.h"
#include "battlefield.h"

class SpikesEffect : public FieldEffect
{
public:
	SpikesEffect() {
		m_message = "Spikes were scattered around the foe's team!";
		m_maxLayers = 3;
	};

	template < typename Type >
	static SpikesEffect* getSpikes(BattleField* field)
	{
		fieldEffects_t effects = field->getEffectsByType<SpikesEffect>();
		if(effects.size() == 0) return NULL;

		fieldEffects_t::iterator it = effects.begin();
		for(; it != effects.end(); ++it)
		{
			SpikesEffect* eff = (SpikesEffect*)(*it);
			if(isAssignableFrom<Type*>(eff)) {
				return eff;
			}
		}

		return NULL;
	}

	virtual std::string getName() { return "Spikes"; };
	std::string getDescription() { return ""; };

	int getTier() { return -1; };

	int getLayers(Pokemon* p) {
		uint8_t team = p->getMaster()->getBattleTeamIdx();
		return m_layers[team];
	};

	void addSpikes(Pokemon* p)
	{
		uint8_t team = p->getMaster()->getBattleTeamIdx();

		int layers = getLayers(p);
		if(layers >= m_maxLayers) {
			p->getField()->showMessage("But it failed!");
			return;
		}

		m_layers[team]++;
		if(m_message.length() > 0) {
			p->getField()->showMessage(m_message);
		}
	};

	void removeSpikes(Pokemon* p) {
		uint8_t team = p->getMaster()->getBattleTeamIdx();
		m_layers[team] = 0;
	};

	virtual void switchIn(Pokemon *p) 
	{
		if(PokemonMove::getEffectiveness(PokemonType::T_GROUND, NULL, p) == 0.0 || p->hasAbility("Levitate") || getLayers(p) <= 0) {
			return;
		}

		int layers = getLayers(p);
		int maximum = p->getStat(S_HP);
		double factor[3] = { 0.125, 0.1875, 0.25 };
		int damage = (int)(((double)maximum) * factor[layers - 1]);
		if(damage < 1) damage = 1;

		std::string message = boost::str(boost::format("%s was hurt by Spikes!") % p->getName());
		p->getField()->showMessage(message);

		p->changeHealth(-damage, true);
	};

	virtual bool applyToField(BattleField* field) {
		field->showMessage("Spikes were scattered everywhere!");
		return true;
	};

	bool tickField(BattleField* field) { return false; };

protected:
	intArray_t m_layers;
	int m_maxLayers;
	std::string m_message;
};

#endif