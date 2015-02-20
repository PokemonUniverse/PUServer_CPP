#ifndef __TILE_H_
#define __TILE_H_

#include <boost/unordered_map.hpp>

#include "definitions.h"
#include "position.h"

class Creature;
class Town;
class TileEvent;

class Tile;
class TileLayer;
typedef std::vector<Tile*> tileList_t;
typedef boost::unordered_map<uint32_t, TileLayer*> LayerMap;

class TileLayer
{
public:
	TileLayer(uint32_t _layer, uint32_t _id) : layer(_layer), tileID(_id) {
		m_eventID = 0;
		m_eventString = "";
	};
	~TileLayer() {};

	uint32_t getTileID() const { return tileID; };

	void setEventID(uint32_t _id) { m_eventID = _id; };
	void setEventData(std::string _event) { m_eventString = _event; };
	
private:
	uint32_t layer;
	uint32_t tileID;

	uint32_t m_eventID;
	std::string m_eventString;
};

class Tile
{
public:
	Tile(Position pos);
	Tile(int _x, int _y, int _z);
	~Tile() {};

	void setBlockingVolume(uint32_t blocking) { 
		m_blockingvolume = blocking; 
		if(blocking == TILEBLOCK_WALK || blocking == TILEBLOCK_SURF) {
			m_activeCreatures.reserve(5);
		}
	};
	uint32_t getBlockingVolume() { return m_blockingvolume; };
	
	Position getPosition() { return m_position; };

	bool addLayer(int id, TileLayer *layer) {
		if(!getLayer(id)) {
			tilelayers.insert(std::make_pair<uint32_t, TileLayer*>(id, layer));

			return true;
		}

		return false;
	};

	TileLayer *getLayer(int layer) { 
		LayerMap::iterator it = tilelayers.find(layer); 
		if(it != tilelayers.end()) {
			return it->second;
		}

		return NULL;
	};

	LayerMap getLayers() { return tilelayers; };

	void setSection(uint32_t _section) { m_sectionID = _section; };
	uint32_t getSection() { return m_sectionID; };

	void setTown(uint32_t _townID);
	Town *getTown() { return m_town; };

	void addTileEvent(TileEvent* _event) { m_event = _event; };
	bool hasTileEvent() { return (m_event != NULL); };
	const TileEvent* getTileEvent() { return m_event; };

	//************************************
	// Method:    checkMovement
	// FullName:  Tile::checkMovement
	// Access:    public 
	// Returns:   ReturnValue
	// Qualifier: Check if creature can move to this tile
	// Parameter: Creature * creature
	// Parameter: Direction dir
	//************************************
	ReturnValue checkMovement(Creature *creature, Direction dir);

	ReturnValue addCreature(Creature *creature);
	ReturnValue removeCreature(Creature *creature);

private:
	Position m_position;

	uint32_t m_blockingvolume;
	uint32_t m_sectionID;

	Town *m_town;
	TileEvent* m_event;

	LayerMap tilelayers;

	typedef std::vector<Creature*> activeCreatures_t;
	activeCreatures_t m_activeCreatures;
};

#endif