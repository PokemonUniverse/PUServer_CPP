#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#include "definitions.h"

#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<uint32_t> list_type;

class AutoID 
{
public:
	AutoID() 
	{
		count++;
		if(count >= 0xFFFFFF)
			count = 1000;

		list_type::iterator find = std::find(list.begin(), list.end(), count);
		if(find != list.end())
		{
			if(count >= 0xFFFFFF)
				count = 1000;
			else
				count++;
		}

		list.push_back(count);
		auto_id = count;
	}

	virtual ~AutoID()
	{
		list_type::iterator find = std::find(list.begin(), list.end(), auto_id);
		if(find != list.end())
			list.erase(find);
	}

	uint32_t auto_id;

protected:
	static uint32_t count;
	static list_type list;
};

#endif