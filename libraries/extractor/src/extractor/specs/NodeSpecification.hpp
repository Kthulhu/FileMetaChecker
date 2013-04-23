#ifndef _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_
#define _EXTRACTOR_SPECS_NODE_SPECIFICATION_HPP_

#include "GroupProperties.hpp"
#include "common.hpp"

#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <map>

class File;

class NodeSpecification
{
public:
	NodeSpecification( File* file );

	bool isValid(SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );

private:
	File*       _file;
	ElementsMap _headerElements;
};

#endif
