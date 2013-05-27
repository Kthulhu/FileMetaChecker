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

	bool isValid(SubSpec& subSpec, GroupProperties& parentProperties, bpt::ptree& nodeReport );

	template< typename DataType >
	void exportValidData( const bool isValid, const std::string& id, const DataType& data, GroupProperties& groupProperties, bpt::ptree& nodeReport, std::string& mapStr );

private:
	bool isValidOrderedGroup       ( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );
	bool oneNodeValidUnorderedGroup( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );
	bool isValidUnorderedGroup     ( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );
	bool isValidSubGroup           ( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport );
	
	File*       _file;
	ElementsMap _headerElements;
};

#include "NodeSpecification.tcc"

#endif
