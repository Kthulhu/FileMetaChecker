#ifndef _SPEC_READER_SPECNODE_HPP_
#define _SPEC_READER_SPECNODE_HPP_

#include <Common/common.hpp>

#include <set>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include <boost/property_tree/ptree.hpp>

namespace spec_reader
{

class Specification;

class SpecNode
{
	typedef boost::property_tree::ptree::const_iterator TreeNodeIt;
public:
	SpecNode( const Specification* spec,
		      const TreeNodeIt node,
		      const SpecNode* parent = nullptr );

	size_t getUId( ) const { return _uId; }

	std::string  getId()          const;
	std::string  getLabel()       const;
	EType        getType()        const;
	EDisplayType getDisplayType() const;
	std::string  getCount()       const;
	std::string  getRequirement() const;
	std::string  getGroupSize()   const;
	
	bool   isGroup()     const;
	bool   isOrdered()   const;
	bool   isOptional()  const;
	bool   isBigEndian() const;
	size_t isRepeated()  const;

	std::vector< std::string >                           getValues()      const;
	std::vector< std::pair< std::string, std::string > > getRange()       const;
	std::vector< std::pair< std::string, std::string > > getRepetitions() const;
	std::map< std::string, std::string >                 getMap()         const;
	
	const SpecNode* getParent() const { return _parent; }

	std::shared_ptr< spec_reader::SpecNode > next      () const;
	std::shared_ptr< spec_reader::SpecNode > firstChild() const;
	
	size_t getChildrenNumber() const;
	std::set< std::string > getChildrenNodes() const;
	
protected:
	TreeNodeIt getIterator() const { return _node; }

private:
	std::string getProperty( const std::string& prop ) const;
	std::string getProperty( const std::string& prop, const std::string& defaultValue ) const;

private:
	size_t               _uId;
	TreeNodeIt           _node;
	const SpecNode*      _parent;
	const Specification* _specification;
	
	static size_t _globalIndex;
};

}

#endif
