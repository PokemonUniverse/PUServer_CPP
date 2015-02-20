#ifndef __DEPOT_H_
#define __DEPOT_H_

#include "container.h"

class Depot : public Container
{
public:
	Depot(uint32_t _type);
	~Depot();
};

#endif