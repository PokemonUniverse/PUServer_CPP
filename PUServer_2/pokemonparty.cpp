#include "pokemonparty.h"

#include "pokemon.h"

void PokemonParty::add( Pokemon *p, uint8_t slot )
{
	if(slot < 0 || slot > 5) { return; }
	boost::mutex::scoped_lock lk(m_mutex);
	m_party.insert(std::make_pair<uint8_t, Pokemon*>(slot, p));
}

void PokemonParty::remove( Pokemon *p )
{
	boost::mutex::scoped_lock lk(m_mutex);
	foreach(pokemonParty_t::value_type it, m_party) {
		Pokemon *tmp;
		if((tmp = it.second) && tmp != NULL && tmp->getGUID() == p->getGUID()) {
			remove(it.first);
			break;
		}
	}

	return;
}

void PokemonParty::remove( uint8_t slot )
{
	if(slot < 0 || slot > 5) { return; }

	m_mutex.lock();
	m_party[slot] = NULL;
	m_mutex.unlock();

	reorder();
}

uint8_t PokemonParty::getSlot( Pokemon *p )
{
	boost::mutex::scoped_lock lk(m_mutex);
	foreach(pokemonParty_t::value_type it, m_party) {
		Pokemon *tmp;
		if((tmp = it.second) && tmp != NULL && tmp->getGUID() == p->getGUID()) {
			return it.first;
		}
	}
	return -1;
}

Pokemon* PokemonParty::get( uint8_t slot )
{
	if(slot < 0 || slot > 5) { return NULL; }
	boost::mutex::scoped_lock lk(m_mutex);
	return m_party[slot];
}

void PokemonParty::move( uint8_t slotFrom, uint8_t slotTo )
{
	if(slotFrom == slotTo || 
		(slotFrom < 0 || slotFrom > 5) || 
		(slotTo < 0 || slotTo > 5)) {
			return;
	}

	boost::mutex::scoped_lock lk(m_mutex);

	Pokemon* save = m_party[slotFrom];
	if(slotTo > slotFrom) // Sort up
	{
		int mover = slotFrom + 1;
		while(mover <= slotTo)
		{
			add(m_party[mover], mover-1);
			mover++;
		}
		add(save, slotTo);
	}
	else if(slotFrom > slotTo) // Sort down
	{
		int mover = slotFrom - 1;
		while(mover >= slotTo)
		{
			add(m_party[mover], mover+1);
			mover--;
		}
		add(save, slotTo);
	}
}

void PokemonParty::reorder()
{
	boost::mutex::scoped_lock lk(m_mutex);
	uint8_t count = 0;
	bool isEmpty = false;
	foreach(pokemonParty_t::value_type it, m_party) 
	{
		Pokemon *tmp = it.second;
		if(tmp) {
			if(isEmpty) {
				m_party[count] = tmp;
				m_party[it.first] = NULL;
			}
			count++;
		}
		else {
			isEmpty = true;
		}
	}
}

Pokemon* PokemonParty::getFirstAvailablePokemon()
{
	foreach(pokemonParty_t::value_type it, m_party) 
	{
		Pokemon *tmp = it.second;
		if(tmp && !tmp->isFainted()) {
			return tmp;
		}
	}

	return NULL;
}

int PokemonParty::getAliveCount()
{
	int alive = 0;
	foreach(pokemonParty_t::value_type it, m_party) 
	{
		Pokemon *tmp = it.second;
		if(tmp && !tmp->isFainted()) {
			alive++;
		}
	}

	return alive;
}