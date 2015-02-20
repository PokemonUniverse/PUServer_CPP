#include "container.h"
#include "game.h"
#include "player.h"

extern Game g_game;

Container::Container(uint16_t _type)// : Item(_type)
{
	//std::cout << "Container constructor " << this << std::endl;
	//maxSize = items[this->getID()].maxItems;
	total_weight = 0.0;
}

Container::~Container()
{
	//std::cout << "Container destructor " << this << std::endl;
	for(ItemList::iterator cit = itemlist.begin(); cit != itemlist.end(); ++cit){		
		(*cit)->setParent(NULL);
		(*cit)->releaseThing2();
	}

	itemlist.clear();
}
/*
Item* Container::clone() const
{
	Container* _item = static_cast<Container*>(Item::clone());
	for(ItemList::const_iterator it = itemlist.begin(); it != itemlist.end(); ++it){
		_item->addItem((*it)->clone());
	}
	return _item;
}*/

Container* Container::getParentContainer() {
	if(Thing* thing = getParent()) {
		if(Item* item = thing->getItem()) {
			return item->getContainer();
		}
	}
	return NULL;
}

void Container::addItem(Item* item)
{
	itemlist.push_back(item);
	item->setParent(this);
}

void Container::updateItemWeight(double diff)
{
	total_weight += diff;
	if(Container* parent_container = getParentContainer()) {
		parent_container->updateItemWeight(diff);
	}
}

double Container::getWeight() const
{
	//return Item::getWeight() + total_weight;
	return 0.0;
}

Item* Container::getItem(uint32_t index)
{
	size_t n = 0;			
	for (ItemList::const_iterator cit = getItems(); cit != getEnd(); ++cit) {
		if(n == index)
			return *cit;
		else
			++n;
	}

	return NULL;
}

uint32_t Container::getItemHoldingCount() const
{
	uint32_t counter = 0;

	for(ContainerIterator iter = begin(); iter != end(); ++iter){
		++counter;
	}

	return counter;
}

bool Container::isHoldingItem(const Item* item) const
{
	for(ContainerIterator cit = begin(); cit != end(); ++cit){
		if(*cit == item)
			return true;
	}
	return false;
}

ContainerIterator Container::begin()
{
	ContainerIterator cit(this);
	if(!itemlist.empty()){
		cit.over.push(this);
		cit.cur = itemlist.begin();
	}
	return cit;
}

ContainerIterator Container::end()
{
	ContainerIterator cit(this);
	return cit;
}

// Very evil constructors, look away if you are sensitive!
ContainerIterator Container::begin() const
{
	Container* evil = const_cast<Container*>(this);
	return evil->begin();
}

ContainerIterator Container::end() const
{
	Container* evil = const_cast<Container*>(this);
	return evil->end();
}

ContainerIterator::ContainerIterator() 
	: super(NULL)
{
}

ContainerIterator::ContainerIterator(Container* super) 
	: super(super) 
{
}

ContainerIterator::~ContainerIterator() 
{
}

ContainerIterator::ContainerIterator(const ContainerIterator& rhs)
	: super(rhs.super), over(rhs.over), cur(rhs.cur) 
{
}

bool ContainerIterator::operator==(const ContainerIterator& rhs)
{
	return !(*this != rhs);
}

bool ContainerIterator::operator!=(const ContainerIterator& rhs)
{
	assert(super);
	if(super != rhs.super)
		return true;
		
	if(over.empty() && rhs.over.empty())
		return false;
	if(over.empty())
		return true;
	if(rhs.over.empty())
		return true;

	if(over.front() != rhs.over.front())
		return true;
	
	return cur != rhs.cur;
}

ContainerIterator& ContainerIterator::operator=(const ContainerIterator& rhs) 
{
	this->super = rhs.super;
	this->cur = rhs.cur;
	this->over = rhs.over;
	return *this;
}

Item* ContainerIterator::operator*() 
{
	assert(super);

	return *cur;
}

Item* ContainerIterator::operator->() 
{
	return *(*this);
}

ContainerIterator& ContainerIterator::operator++() 
{
	assert(super);
	
	Item* i = *cur;
	Container* c = i->getContainer();
	if(c && !c->itemlist.empty())
		over.push(c);
	
	++cur;
	
	if(cur == over.front()->itemlist.end()){
		over.pop();
		if(over.empty()){
			return *this;
		}
		cur = over.front()->itemlist.begin();
	}
	
	return *this;
}

ContainerIterator ContainerIterator::operator++(int) 
{
	ContainerIterator tmp(*this);
	++*this;
	return tmp;
}
