#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <queue>

#include "definitions.h"
#include "cylinder.h"
#include "items.h"

class Container;
//class Depot;

class ContainerIterator
{
public:
	ContainerIterator();
	ContainerIterator(const ContainerIterator& rhs);
	~ContainerIterator();

	ContainerIterator& operator=(const ContainerIterator& rhs);
	bool operator==(const ContainerIterator& rhs);
	bool operator!=(const ContainerIterator& rhs);
	ContainerIterator& operator++();
	ContainerIterator operator++(int);
	Item* operator*();
	Item* operator->();

protected:
	ContainerIterator(Container* super);

	Container* super;
	std::queue<Container*> over;
	ItemList::iterator cur;

	friend class Container;
};

class Container : public Item, public Cylinder
{
public:
	Container(uint16_t _type);
	virtual ~Container();
	//virtual Item* clone() const;

	virtual Container* getContainer() {return this;};
	virtual const Container* getContainer() const {return this;};
	//virtual Depot* getDepot() {return NULL;};
	//virtual const Depot* getDepot() const {return NULL;};

	//serialization
	//bool unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream);

	uint32_t size() const {return (uint32_t)itemlist.size();};
	uint32_t capacity() const {return maxSize;};

	ContainerIterator begin();
	ContainerIterator end();
	ContainerIterator begin() const;
	ContainerIterator end() const;

	ItemList::const_iterator getItems() const {return itemlist.begin();}
	ItemList::const_iterator getEnd() const {return itemlist.end();}
	ItemList::const_reverse_iterator getReversedItems() const {return itemlist.rbegin();}
	ItemList::const_reverse_iterator getReversedEnd() const {return itemlist.rend();}

	void addItem(Item* item);
	Item* getItem(uint32_t index);
	bool isHoldingItem(const Item* item) const;

	uint32_t getItemHoldingCount() const;
	virtual double getWeight() const;

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
	virtual void __startDecaying() {}

private:
	Container* getParentContainer();
	void updateItemWeight(double diff);


protected:
	uint32_t maxSize;
	double total_weight;
	ItemList itemlist;

	friend class ContainerIterator;
};

#endif

