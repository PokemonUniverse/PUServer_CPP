#ifndef __TOOLS_H_
#define __TOOLS_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <time.h>
#include <sys/timeb.h>

#include <libxml/parser.h>

#include "definitions.h"

enum DistributionType_t{
	DISTRO_UNIFORM,
	DISTRO_SQUARE,
	DISTRO_NORMAL
};

inline int64_t PUSYS_TIME()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

template < class C, class U > 
inline bool isAssignableFrom(U u) {
	return dynamic_cast< C >(u) != NULL;
}

void align_right(std::string source);

void trim_right(std::string& source, const std::string& t);
void trim_left(std::string& source, const std::string& t);

std::vector<std::string> explodeString(const std::string& inString, const std::string& separator);
void replaceString(std::string& str, const std::string sought, const std::string replacement);

// String transformation
void toLowerCaseString(std::string& source);
void toUpperCaseString(std::string& source);
std::string asLowerCaseString(const std::string& source);
std::string asUpperCaseString(const std::string& source);
char upchar(char c);

bool hasBitSet(uint32_t flag, uint32_t flags);
int random_range(int lowest_number, int highest_number, DistributionType_t type = DISTRO_NORMAL);

// Xml
bool readXMLInteger(xmlNodePtr node, const char* tag, int& value);
/*#if (defined __WINDOWS__ || defined WIN32) && !defined __GNUC__
bool readXMLInteger(xmlNodePtr node, const char* tag, int32_t& value);
#endif*/
bool readXMLInteger64(xmlNodePtr node, const char* tag, uint64_t& value);
bool readXMLFloat(xmlNodePtr node, const char* tag, float& value);
bool readXMLString(xmlNodePtr node, const char* tag, std::string& value);
bool readXMLContentString(xmlNodePtr node, std::string& value);

bool passwordTest(const std::string &plain, std::string &hash);
#endif