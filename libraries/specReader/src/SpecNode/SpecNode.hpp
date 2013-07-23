#ifndef _SPECREADER_SPECNODE_HPP_
#define _SPECREADER_SPECNODE_HPP_

#include <common/global.hpp>

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

const std::string kId     = "id";
const std::string kLabel  = "label";

const std::string kAscii  = "ascii";
const std::string kHexa   = "hexa";
const std::string kType   = "type";
const std::string kCount  = "count";
const std::string kValues = "values";
const std::string kMap    = "map";
const std::string kRange  = "range";
const std::string kMin    = "min";
const std::string kMax    = "max";

const std::string kGroup         = "group";
const std::string kGroupSize     = "groupSize";

const std::string kRepetition    = "repeated";
const std::string kRequired      = "required";

const std::string kEndian        = "endian";
const std::string kEndianBig     = "big";
const std::string kEndianLittle  = "little";

const std::string kOptional      = "optional";
const std::string kOptionalTrue  = "true";
const std::string kOptionalFalse = "false";

const std::string kOrdered       = "ordered";
const std::string kOrderedTrue   = "true";
const std::string kOrderedFalse  = "false";

const std::string kData          = "data";
const std::string kDisplayType   = "displayType";

namespace spec_reader
{

class SpecNode
{
public:
	
	SpecNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal );
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

	size_t getIndex();
	size_t getIndexTotal();

protected:
	std::string getProperty( const std::string& prop );
	std::string getProperty( const std::string& prop, const std::string& defaultValue );

private:
	bpt::ptree::const_iterator _node;
	size_t _index;
	size_t _indexTotal;
};

}

#endif
