#ifndef INC_IOMAP_V2_H_
#define INC_IOMAP_V2_H_

#include "definitions.h"

#include "iomap.h"

class IOMap_V2 : public IOMap
{
public:
	IOMap_V2() {};
	~IOMap_V2() {};

	std::string getSourceDescription() { return "XML V2.5"; };
	bool loadMap(Map* map);

private:
	bool readMap(Map* map, const std::string& identifier);
};

#endif