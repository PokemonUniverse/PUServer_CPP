#include "pokecenter.h"

PokeCenter::PokeCenter()
{

}

PokeCenter::~PokeCenter()
{
}


void PokeCenter::healActivePokemon(Player *player)
{
	PokemonList healParty = player->getPokemonParty();
	for (PokemonList::iterator it = healParty.begin(); it != healParty.end(); ++it)
	{
		if(it->second)
		{	
			std::stringstream healQuery;
			healQuery << "UPDATE user_pokemon p, user_pokemon_moves m SET"
				<< " p.pkmn_hp='0', p.pkmn_status='0', m.moveUsage='0'"
				<< " WHERE p.uniqueID='" << it->second->getPokemonID() << "' AND m.uniqueID='" << it->second->getPokemonID() << "'";

			player->database->runQuery(healQuery.str().c_str());

			it->second->heal();
		}
	}

	player->setActivePokemon();
	player->client->sendPokemonData(player->getPokemonParty());
}