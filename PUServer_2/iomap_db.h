#ifndef INC_IOMAP_DB_H_
#define INC_IOMAP_DB_H_

#include "definitions.h"

#include "iomap.h"

class IOMap_DB : public IOMap
{
public:
	IOMap_DB() {};
	~IOMap_DB() {};

	std::string getSourceDescription() { return "Database"; };
	bool loadMap(Map* map);
};

#endif