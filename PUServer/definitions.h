#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <iostream>

using namespace std;

#ifdef XML_GCC_FREE
	#define xmlFreePUSERV(s)	free(s)
#else
	#define xmlFreePUSERV(s)	xmlFree(s)
#endif

#ifdef __GNUC__
	#include <assert.h>
	#define ATOI64 atoll
#else
	#include <limits>
	#include <assert.h>
	#include <time.h>

	#include <cstring>
	inline int strcasecmp(const char *s1, const char *s2)
	{
		return ::_stricmp(s1, s2);
	}

	inline int strncasecmp(const char *s1, const char *s2, size_t n)
	{
		return ::_strnicmp(s1, s2, n);
	}

	#define ATOI64 _atoi64
#endif

#if defined WIN32 || _WIN32
	#define __BUGTRAP__
#endif

typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef int mapIndex_t;

// Pokemon Universe Stuff
#define PUSERV_VERSION "0.9.4.1"
#define PUSERV_NAME "PUServer"
#define PUSERV_CLIENT_VERSION 4

#define VIEWPORT_X 28
#define VIEWPORT_Y 22
#define VIEWPORT_X_CENTER 14
#define VIEWPORT_Y_CENTER 11
#define MAP_LAYERS 3

#define PUSERV_PKMN_LVLBLOCK 20
#define PUSERV_CHAT_SPAM 1000

#define __PU_DEBUG__
#define __USE_MYSQL__
//#define __USE_LUALIBRARIES__

#endif