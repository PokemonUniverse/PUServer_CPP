#include "global.h"

#include "cylinder.h"

VirtualCylinder* VirtualCylinder::virtualCylinder = new VirtualCylinder;

int32_t Cylinder::__getIndexOfThing(const Thing* thing) const
{
	return -1;
}

int32_t Cylinder::__getFirstIndex() const
{
	return -1;
}

int32_t Cylinder::__getLastIndex() const
{
	return -1;
}

uint32_t Cylinder::__getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/, bool itemCount /*= true*/) const
{
	return 0;
}

Thing* Cylinder::__getThing(uint32_t index) const
{
	return NULL;
}

void Cylinder::__internalAddThing(Thing* thing)
{
	//
}

void Cylinder::__internalAddThing(uint32_t index, Thing* thing)
{
	//
}

void Cylinder::__startDecaying()
{
	//
}