#ifndef __THING_H_
#define __THING_H_

#include "definitions.h"
#include "position.h"

class Tile;
class Cylinder;
class Item;
class Creature;

class Thing
{
protected:
	Thing() {};

public:
	virtual ~Thing() {};

	void useThing() { ++useCount; }
	void releaseThing() {
		--useCount;
		if(useCount <= 0) {
			delete this;
		}
	}

	virtual std::string getDescription(int32_t lookDistance) const = 0;

	Cylinder* getParent() {return parent;};
	const Cylinder* getParent() const {return parent;};

	virtual void setParent(Cylinder* cylinder) {parent = cylinder;};

	Cylinder* getTopParent() { return NULL; }; //returns Tile/Container or a Player
	const Cylinder* getTopParent() const { return NULL; };

	virtual Tile* getTile() { return NULL; };
	virtual const Tile* getTile() const { return NULL; };

	virtual const Position getPosition() const { return Position::PosNULL(); };
	virtual int getThrowRange() const = 0;

	virtual Item* getItem() { return NULL; };
	virtual const Item* getItem() const { return NULL; };
	virtual Creature* getCreature() { return NULL; };
	virtual const Creature* getCreature() const { return NULL; };

	virtual bool isRemoved() const { return false; };

private:
	Cylinder* parent;
	int32_t useCount;
};

#endif