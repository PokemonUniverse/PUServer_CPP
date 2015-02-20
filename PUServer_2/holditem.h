#ifndef INC_HOLDITEM_H_
#define INC_HOLDITEM_H_

#include <boost/unordered_map.hpp>
#include <vector>

#include "definitions.h"
#include "intrinsicability.h"

#include "pokemon.h"

typedef std::vector<Pokemon*> pokemonList_t;
typedef std::vector<std::string> itemSet_t;

class HoldItemData
{
	typedef boost::unordered_map<std::string, itemSet_t> exclusiveList_t;
	exclusiveList_t m_exclusive;

	itemSet_t m_items;

public:
	HoldItemData() {};

	void addItem(std::string name) { m_items.push_back(name); };

	//************************************
	// Method:    addExclusiveItem
	// FullName:  HoldItemData::addExclusiveItem
	// Access:    public 
	// Returns:   void
	// Qualifier: Add an exclusive item to a pokemon.
	// Parameter: std::string name
	// Parameter: std::string pokemon
	//************************************
	void addExclusiveItem(std::string name, std::string pokemon);
	
	//************************************
	// Method:    removeExclusiveItem
	// FullName:  HoldItemData::removeExclusiveItem
	// Access:    public 
	// Returns:   void
	// Qualifier: Remove an exclusive item from a pokemon.
	// Parameter: std::string name
	// Parameter: std::string pokemon
	//************************************
	void removeExclusiveItem(std::string name, std::string pokemon);

	//************************************
	// Method:    getItemSet
	// FullName:  HoldItemData::getItemSet
	// Access:    public 
	// Returns:   itemSet_t
	// Qualifier: Get an item set corresponding to the named species.
	// Parameter: std::string pokemon
	//************************************
	itemSet_t getItemSet(std::string pokemon);

	//************************************
	// Method:    canUseitem
	// FullName:  HoldItemData::canUseitem
	// Access:    public 
	// Returns:   bool
	// Qualifier: Return whether the named species can use a particular item.
	// Parameter: std::string pokemon
	// Parameter: std::string item
	//************************************
	bool canUseitem(std::string pokemon, std::string item);
};

class HoldItem : public IntrinsicAbility
{
private:
	static HoldItemData* m_default;

protected:
	void registerAbility();

public:
	HoldItem(std::string name) : IntrinsicAbility(name) {};
	HoldItem(std::string name, pokemonList_t pokemon);
	HoldItem(std::string name, bool reg) : IntrinsicAbility(reg, name) {};

	virtual ~HoldItem() {};

	static HoldItemData* getDefaultData() { return m_default; };
};

#endif