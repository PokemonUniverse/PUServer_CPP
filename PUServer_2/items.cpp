#include "items.h"

bool Items::load()
{
	return true;
}

bool Items::reload()
{
	if(m_isLoaded) {
		clear();
	}

	return load();
}

void Items::clear()
{

}