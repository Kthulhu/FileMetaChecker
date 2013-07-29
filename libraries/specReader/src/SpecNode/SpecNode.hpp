#ifndef _SPECREADER_SPECNODE_HPP_
#define _SPECREADER_SPECNODE_HPP_

#include <common/global.hpp>

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

namespace spec_reader
{

class SpecNode
{
public:
	
	SpecNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal );
	SpecNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal, SpecNode* parent );
	~SpecNode();

	std::string getId();
	std::string getLabel();
	std::string getType();
	std::string getDisplayType();

	std::string getCount();
	std::string getRequired();

	std::vector< std::string > getValues();

	std::vector< std::pair< std::string, std::string > > getRange();
	std::vector< std::pair< std::string, std::string > > getRepetition();

	std::map< std::string, std::string > getMap();

	bool isBigEndian();
	bool isOptional();
	bool isOrdered();

	bool hasGroup();
	std::string getGroupSize();

	SpecNode next();
	SpecNode firstChild();
	SpecNode* parent();	// @todo

	size_t getIndex();
	size_t getIndexTotal();

protected:
	std::string getProperty( const std::string& prop );
	std::string getProperty( const std::string& prop, const std::string& defaultValue );

private:
	bpt::ptree::const_iterator _node;
	SpecNode* _parent;	// @todo
	size_t _index;
	size_t _indexTotal;
};

}

#endif
