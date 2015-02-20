#ifndef INC_IOMAP_H_
#define INC_IOMAP_H_

#include <string>

#include "definitions.h"

class Map;

class IOMap
{
public:
	IOMap() {};
	virtual ~IOMap() {};

	virtual std::string getSourceDescription() = 0;
	virtual bool loadMap(Map* map) = 0;

	void setLastErrorMessage(std::string message) { m_lastErrorMessage = message; };
	std::string getLastErrorMessage() { return m_lastErrorMessage; };

protected:
	std::string m_lastErrorMessage;
};
#endif