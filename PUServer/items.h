#ifndef __items_h_
#define __items_h_

#include <iostream>
#include <string>
#include <sstream>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "thing.h"
#include "database.h"

extern Database g_db;

class Container;

typedef boost::unordered_map<int, Item*> ItemsList;
typedef std::list<Item*> ItemList;

enum Item_Category
{
	ITEM_POWERUPS = 0,
	ITEM_BERRIES = 1,
	ITEM_EQUIPMENT = 2,
	ITEM_POKEBALL = 3,
	ITEM_HELD = 4,
	ITEM_QUEST = 5,
	ITEM_MISC = 6,
};

class Item : virtual public Thing
{
public:
	Item();
	Item(int _id, std::string _name, std::string _description, int _count, int _berry_id);

	~Item() {};

	Item* getItem() { return this; }
	const Item* getItem() const {  return this; }

	Item* clone();

	virtual Container* getContainer() { return NULL; }
	virtual const Container* getContainer() const { return NULL; }

	void setID(int _id) { id = _id; }
	void setCount(int _count) { count = _count; }
	void setCost(int _cost) { cost = _cost; }
	void setPlusHP(int _hp) { plusHP = _hp; }
	void setPlusPP(int _pp) { plusPP = _pp; }
	void setFlingPower(int _flingPower) { flingPower = _flingPower; }
	void setNaturalGiftPower(int _naturalPower) { naturalGiftPower = _naturalPower; }
	void setNaturalGiftType(int _naturalType) { naturalGiftType = _naturalType; }

	void setEffort(int _hp, int _at, int _de, int _sa, int _sd, int _sp) {
		effort_hp = _hp;
		effort_at = _at;
		effort_de = _de;
		effort_sa = _sa;
		effort_sd = _sd;
		effort_sp = _sp;
	}

	void setHappiness(int _1, int _2, int _3) {
		happiness1 = _1;
		happiness2 = _2;
		happiness3 = _3;
	}
	
	void setName(std::string _name) { name = _name; }
	void setDescription(std::string _description) { description = _description; }
	void setFlingDescription(std::string _description) { flingDescription = _description; }
	void setCategory(std::string _category);

	int getID() { return id; }
	int getCount() { return count; }
	int getCost() { return cost; }
	int getPlusHP() { return plusHP; }
	int getPlusPP() { return plusPP; }
	int getFlingPower() { return flingPower; }
	int getNaturalGiftPower() { return naturalGiftPower; }
	int getNaturalGiftType() { return naturalGiftType; }

	int getEffortHP() { return effort_hp; }
	int getEffortAT() { return effort_at; }
	int getEffortDE() { return effort_de; }
	int getEffortSA() { return effort_sa; }
	int getEffortSD() { return effort_sd; }
	int getEffortSP() { return effort_sp; }

	int getHappiness1() { return happiness1; }
	int getHappiness2() { return happiness1; }
	int getHappiness3() { return happiness1; }

	std::string getName() { return name; }
	std::string getDescription() { return description; }
	std::string getFlingDescription() { return flingDescription; }
	int getCategoryID() { return categoryID; }
	std::string getCategoryName() { return category; }

	static int getCategoryByName(std::string _name);
	static std::string getCategoryByID(int _id);
	
	// Methods for Berries
	void setSize(int _size) { size = _size; }
	void setFirmness(std::string _firmness) { firmness = _firmness; }
	void setMaxHarvest(int _max_harvest) { max_harvest = _max_harvest; }
	void setGrowthTime(int _growth_time) { growth_time = _growth_time; }
	void setSoilDryness(int _soil_dryness) { soil_dryness = _soil_dryness; }
	void setSpicy(int _spicy) { spicy = _spicy; }
	void setDry(int _dry) { dry = _dry; }
	void setSweet(int _sweet) { sweet = _sweet; }
	void setBitter(int _bitter) { bitter = _bitter; }
	void setSour(int _sour) { sour = _sour; }
	void setSmoothness(int _smoothness) { smoothness = _smoothness; }

	int getSize() { return size; }
	std::string getFirmness() { return firmness; }
	int getMaxHarvest() { return max_harvest; }
	int getGrowthTime() { return growth_time; }
	int getSoilDryness() { return soil_dryness; }
	int getSpicy() { return spicy; }
	int getDry() { return dry; }
	int getSweet() { return sweet; }
	int getBitter() { return bitter; }
	int getSour() { return sour; }
	int setSmoothness() { return smoothness; }

private:
	int id;
	int count;
	int cost;
	int plusHP;
	int plusPP;
	int flingPower;
	int naturalGiftPower;
	int naturalGiftType;

	int effort_hp;
	int effort_at;
	int effort_de;
	int effort_sp;
	int effort_sa;
	int effort_sd;

	int happiness1, happiness2, happiness3;

	std::string name;
	std::string description;
	std::string category;
	int categoryID;
	std::string flingDescription;
	
	// Berry variables
	int berry_id;
	int size;
	int max_harvest;
	int growth_time;
	int soil_dryness;
	int spicy;
	int dry;
	int sweet;
	int bitter;
	int sour;
	int smoothness;

	std::string firmness;
};

class Items
{
public:
	Items();
	~Items();

	void clear();
	bool reload();

	ItemsList getItems() { return items; }
	Item* getItem(int _id);
	
	static int createIndex(int _slot, int _category) { return ((_slot) | ((_category) << 8)); }

private:
	ItemsList items;
	bool isLoaded;
};

#endif