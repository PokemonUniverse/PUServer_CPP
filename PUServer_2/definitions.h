#ifndef __PUSERV_DEFINITIONS_H__
#define __PUSERV_DEFINITIONS_H__

#include <iostream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#ifdef XML_GCC_FREE
#define xmlFreePUSERV(s)	free(s)
#else
#define xmlFreePUSERV(s)	xmlFree(s)
#endif

#define foreach BOOST_FOREACH

#define PUSERV_VERSION "0.0.1_SVN"
#define PUSERV_NAME "PUServer"
#define PUSERV_CLIENT_VERSION 4
#define CURRENT_SCHEMA_VERSION 1

#define VIEWPORT_X 28
#define VIEWPORT_Y 22
#define VIEWPORT_X_CENTER (VIEWPORT_X/2)
#define VIEWPORT_Y_CENTER (VIEWPORT_Y/2)

#define __USE_MYSQL__

#define __IS_DEBUG__ true

typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#define ATOI64 _atoi64

typedef std::vector<bool> boolArray_t;
typedef std::vector<int> intArray_t;
typedef std::vector<double> doubleArray_t;

#include "enums.h"
#include "teamInfo.h"

#endif