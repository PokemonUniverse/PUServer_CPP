#include "towns.h"

#include "database.h"

Towns* Towns::m_instance = new Towns();

bool Towns::load()
{
	Database* db = Database::getFactory();
	DBQuery selectTowns;
	DBResult* result;

	selectTowns << "SELECT t.idlocation, t.name, t.idmusic, p.position FROM location t LEFT JOIN pokecenter p ON p.idpokecenter = t.idpokecenter";
	if((result = db->storeQuery(selectTowns.str())) == NULL) {
		return false;
	}
	
	if(result->count() > 0)
	{
		do
		{
			int id = result->getDataInt("idlocation");
			int music = result->getDataInt("idmusic");

			std::string name = result->getDataString("name");
			Position pos = Position(result->getDataString("position"));

			Town *newTown = new Town(id, name);
			newTown->setMusic(music);
			newTown->setPokeCenter(pos);

			addTown(id, newTown);
		} while(result->next());
	}
	m_isLoaded = true;

	db->freeResult(result);

	return m_isLoaded;
}

Towns::~Towns()
{

}

Town* Towns::getTown( uint32_t _townid )
{
	TownList::iterator it = townlist.find(_townid);
	if(it != townlist.end()) {
		return it->second;
	}

	return NULL;
}

void Towns::addTown( uint32_t _townid, Town* _town )
{
	townlist.insert(std::make_pair<uint32_t, Town*>(_townid, _town));
}