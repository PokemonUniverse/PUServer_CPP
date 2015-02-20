#ifndef __tile_h_
#define __tile_h_

#include <vector>
#include "global.h"
#include "cylinder.h"
#include "position.h"

class Creature;
class Player;

class TileObject
{
public:
	TileObject(int _z, int _tile);
	TileObject(int _z, int _tile, int _movement);
	~TileObject();

	int getZ() { return z; }
	long getTileID() { return tileID; }
	int getMovement() { return movement; }
	int getEventID() { return eventID; }
	char* getEventData() { return eventData; }

	void setTileID(long _tileID) { tileID = _tileID; }
	void setMovement(int _movement) { movement = _movement; }
	void setEventID(int _eventID) { eventID = _eventID; }
	void setEventData(char* _eventData) { eventData = _eventData; }

private:
	int z, movement, eventID;
	long tileID;
	char* eventData;
};

class Tile : public Cylinder
{
public:
	static Tile null_tile;

	Tile(int _x, int _y);
	Tile(int _x, int _y, int _z);
	~Tile();

	const int getX() const { return pos.x; }
	const int getY() const { return pos.y; }
	const int getZ() const { return pos.z; }

	const Position getPosition() const { return pos; }

	void setPosition(int _x, int _y, int _z) {
		pos.x;
		pos.y;
		pos.z;
	}

	int getTown() { return town; }
	std::string getTownName() { return townName; }
	int getSection() { return section; }
	int getPokeCenterID() { return pokeCenter; }

	void setInside(bool _inside) { pos.z = _inside; }
	void setTown(int _town) { town = _town; }
	void setTownName(std::string _townName) { townName = _townName; }
	void setPokeCenterID(int _pokeCenter) { pokeCenter = _pokeCenter; }
	void setSection(int _section) { section = _section; }

	void addObject(TileObject *obj);
	void removeObject(TileObject *obj);
	TileObject *getObject(int layer) { return objectList[layer]; }
	
	std::vector<Creature*> players;
	void addCreature(Creature *creature);
	void removeCreature(Creature *creature);
	Creature* getFirstCreature();
	
	// Returns true if it has a warp event
	bool moveCreature(Creature *creature, Direction dir, Cylinder *toCynlinder);
	ReturnValue checkMovement(Creature *creature, Direction dir);

	//cylinder implementations
	ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		uint32_t flags) const { return RET_NOERROR; }
	ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
		uint32_t& maxQueryCount, uint32_t flags) const { return RET_NOERROR; }
	ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const { return RET_NOERROR; }
	Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem, uint32_t& flags) { return NULL; }

	void __addThing(Thing* thing) {}
	void __addThing(int32_t index, Thing* thing) {}

	void __updateThing(Thing* thing, uint16_t itemId, uint32_t count) {}
	void __replaceThing(uint32_t index, Thing* thing) {}

	void __removeThing(Thing* thing, uint32_t count) {}

	int32_t __getIndexOfThing(const Thing* thing) const { return -1; }
	int32_t __getFirstIndex() const { return -1; }
	int32_t __getLastIndex() const { return -1; }
	uint32_t __getItemTypeCount(uint16_t itemId, int32_t subType = -1, bool itemCount = true) const { return -1; }
	Thing* __getThing(uint32_t index) const { return NULL; }

	void postAddNotification(Thing* thing, int32_t index, cylinderlink_t link = LINK_OWNER) {}
	void postRemoveNotification(Thing* thing, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER) {}

	void __internalAddThing(Thing* thing) {}
	void __internalAddThing(uint32_t index, Thing* thing) {}

private:
	int town, pokeCenter, section;
	std::string townName;

	Position pos;

	TileObject *objectList[3];
};

#endif