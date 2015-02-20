#ifndef __TOWNS_H_
#define __TOWNS_H_

#include <string>
#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "position.h"

class Town
{
public:
	Town(uint32_t _id, std::string _name) {
		m_id = _id;
		m_name = _name;
		
		m_music = 0;
		m_pokecenter = Position::PosNULL();
	};
	~Town() {};

	void setMusic(uint32_t _music) { m_music = _music; };
	void setPokeCenter(Position _pos) { m_pokecenter = _pos; };

	uint32_t getID() { return m_id; };
	uint32_t getMusic() { return m_music; };
	
	std::string getName() { return m_name; };
	Position getPokeCenter() { return m_pokecenter; };
	
private:
	uint32_t m_id;
	uint32_t m_music;

	std::string m_name;

	Position m_pokecenter;
};

typedef boost::unordered_map<uint32_t, Town*> TownList;
class Towns
{
public:
	Towns() : m_isLoaded(false) { };
	~Towns();

	static Towns* getInstance()
	{
		if(!m_instance) m_instance = new Towns();
		return m_instance;
	};

	TownList::const_iterator getTownBegin() const { return townlist.begin(); };
	TownList::const_iterator getTownEnd() const { return townlist.end(); };

	bool load();

	Town* getTown(uint32_t _townid);
	void addTown(uint32_t _townid, Town* _town);

private:
	static Towns* m_instance;

	bool m_isLoaded;

	TownList townlist;
};

#endif