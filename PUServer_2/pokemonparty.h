#ifndef __POKEMONPARTY_H_
#define __POKEMONPARTY_H_

#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include "definitions.h"

class Pokemon;

class PokemonParty
{
public:
	typedef boost::unordered_map<uint8_t, Pokemon*> pokemonParty_t;

	/************************************************************************/
	/* Stuf for PokemonParty iterator                                       */
	/************************************************************************/
	typedef pokemonParty_t::iterator iterator;
	typedef pokemonParty_t::const_iterator const_iterator;

	iterator begin() { return m_party.begin(); };
	const_iterator begin() const { return m_party.begin(); };

	iterator end() { return m_party.end(); };
	const_iterator end() const { return m_party.end(); };
	/************************************************************************/

	PokemonParty() { };
	~PokemonParty() { };

	pokemonParty_t getParty() { return m_party; };

	void add(Pokemon *p, uint8_t slot);;

	void remove(Pokemon *p);;
	void remove(uint8_t slot);;

	uint8_t getSlot(Pokemon *p);;

	Pokemon* get(uint8_t slot);;

	void move(uint8_t slotFrom, uint8_t slotTo);;

	void reorder();;

	Pokemon* getActivePokemon() 
	{ 
		if(!m_active) {
			m_active = getFirstAvailablePokemon();
		}

		return m_active; 
	};

	void setActivePokemon(Pokemon* p) {
		m_active = p;
	};

	Pokemon* getFirstAvailablePokemon();;

	int getAliveCount();

private:
	boost::mutex m_mutex;
	pokemonParty_t m_party;
	Pokemon* m_active;
};
#endif