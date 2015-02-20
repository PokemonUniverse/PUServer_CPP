#include "depot.h"
#include "game.h"

#include <boost/foreach.hpp>

extern Game g_game;

Depot::Depot( int _userId, int _maxItems /* = 50 */ )
{
	userId = _userId;
	maxItems = _maxItems;
}

Depot::~Depot()
{

}

bool Depot::addItem( int _itemId, int _count /* = 1 */, int _slot /* = -1 */ )
{
	if(getMaxItems() != 99999 && ((getStorageCount() + _count) > getMaxItems())) {
		return false;
	}

	Item *item = g_game.getItem(_itemId);
	if(!item) {
		return false;
	}

	int category = item->getCategoryID();

	Item *newItem = item->clone();
	newItem->setCount(_count);

	int itemIndex = Items::createIndex(_slot, category);
	items.insert(std::pair<int,Item*>(itemIndex, newItem));		

	return true;
}

void Depot::removeItem( int _itemIndex )
{
	Item *item = getItem(_itemIndex);
	if(!item) {
		return;
	}

	items.erase(_itemIndex);
}

bool Depot::updateItem( int _itemIndex, int _count )
{
	if(getMaxItems() != 99999 && ((getStorageCount() + _count) > getMaxItems())) {
		return false;
	}

	Item *item = getItem(_itemIndex);
	if(!item) {
		return false;
	}

	item->setCount(item->getCount() + _count);

	if(item->getCount() <= 0) {
		removeItem(_itemIndex);
	}

	return true;
}

Item *Depot::getItem(int category, int slot)
{
	return getItem(Items::createIndex(slot, category));
}

Item *Depot::getItem( int _itemIndex )
{
	BOOST_FOREACH(ItemMap::value_type it, items)
	{
		int key = it.first;

		if(key == _itemIndex) {
			return it.second;
		}
	}

	return NULL;
}

int Depot::getStorageCount()
{
	int totalStorage = 0;
	BOOST_FOREACH(ItemMap::value_type it, items)
	{
		Item *tmp = it.second;
		totalStorage += tmp->getCount();
	}

	return totalStorage;
}

int Depot::searchItem( int _category, int _itemId )
{
	for(int i = 0; i < 54; i++)
	{
		Item *item = NULL;
		if(item = getItem(Items::createIndex(i, _category) ) )
		{
			if(item->getID() == _itemId) {
				return i;
			}
		}
	}

	return -1;
}

int Depot::getCategoryFreeSlot( int _category )
{
	for(int i = 0; i < 54; i++)
	{
		if(!getItem(Items::createIndex(i, _category) ) ) {
			return i;
		}
	}

	return -1;
}

void Depot::switchItem( int _category, int _oldSlot, int _newSlot )
{
	int indexOld = Items::createIndex(_oldSlot, _category);
	int indexNew = Items::createIndex(_newSlot, _category);

	Item *itemOld = (getItem(indexOld)) ? getItem(indexOld)->clone() : NULL;
	Item *itemNew = (getItem(indexNew)) ? getItem(indexNew)->clone() : NULL;

	if(itemOld && itemNew) {
		items.erase(indexOld);
		items.erase(indexNew);
		items.insert(std::pair<int, Item*>(indexOld, itemNew));
		items.insert(std::pair<int, Item*>(indexNew, itemOld));
	}
	else if(itemOld) {
		items.insert(std::pair<int, Item*>(indexNew, itemOld));
		items.erase(indexOld);
	}
	else if(indexNew) {
		items.insert(std::pair<int, Item*>(indexOld, itemNew));
		items.erase(indexNew);
	}
}