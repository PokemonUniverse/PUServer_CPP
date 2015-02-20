#ifndef __DEPOT_H_
#define __DEPOT_H_

#include "items.h"

class Items;
class Item;

typedef boost::unordered_map<int,Item*> ItemMap;

class Depot
{
public:
	Depot(int _userId, int _maxItems = 50);
	~Depot();

	int getOwer() { return userId; }

	void setMaxItems(int _maxItems) { maxItems = _maxItems; }
	int getMaxItems() { return maxItems; }

	bool addItem(int _itemId, int _count = 1, int slot = -1);
	void removeItem(int _itemIndex);
	bool updateItem(int _itemIndex, int _count);
	Item* getItem(int _itemIndex);
	Item *getItem(int category, int slot);
	int getCategoryFreeSlot(int _category);
	int searchItem(int _category, int _itemId);
	void switchItem(int _category, int _oldSlot, int _newSlot);

	int getStorageCount();

	ItemMap getItemList() { return items; }

private:
	int userId;
	int maxItems;

	ItemMap items;
};

#endif