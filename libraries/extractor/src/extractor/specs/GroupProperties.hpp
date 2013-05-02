#ifndef _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_
#define _EXTRACTOR_SPECS_GROUP_PROPERTIES_HPP_

#include <iostream>

class GroupProperties
{
public:
	GroupProperties()
	{
		_size = 0;
	}

	void addSize( int value)
	{
		_size += value;
	}

	size_t getSize()
	{
		return _size;
	}

private:
	size_t _size;
};

#endif