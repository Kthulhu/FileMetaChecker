#ifndef _BASIC_ELEMENT_ELEMENT_HPP_
#define _BASIC_ELEMENT_ELEMENT_HPP_

#include <Common/common.hpp>

#include <set>
#include <string>
#include <vector>
#include <memory>

namespace spec_reader
{
	class SpecNode;
}

namespace basic_element
{

class Element
{
	struct Properties
	{
		std::string   id;
		std::string   label;
		
		size_t        uId;
		size_t        size;
		size_t        iteration;
		
		std::string   countExpr;
		std::string   requiredExpr;
		std::string   groupSizeExpr;
		
		std::vector< std::string > values;
		std::vector< std::pair< std::string, std::string > > rangeExpr;
		std::vector< std::pair< std::string, std::string > > repetExpr;
		std::map< std::string, std::string > map;

		EType         type;
		ESubType      subType;
		EDisplayType  displayType;
		
		EStatus       status;
		
		char*         data;
		
		bool          isGroup;
		bool          isOrdered;
		bool          isOptional;
		bool          bigEndianData;
		
		std::string   error;
		std::string   warning;
	};

public:
	Element( const spec_reader::SpecNode* node, 
		     const std::shared_ptr< Element > previous = std::shared_ptr< Element >() );
	
	const spec_reader::SpecNode* next( );
	
	std::string getId()    const { return _prop.id; }
	std::string getLabel() const { return _prop.label; }

	size_t      getUniqueId()  const { return _prop.uId; }
	size_t      getSize()      const { return _prop.size; }
	size_t      getIteration() const { return _prop.iteration; }
	
	std::string getCount()       const { return _prop.countExpr; }
	std::string getRequirement() const { return _prop.requiredExpr; }
	std::string getGroupSize()   const { return _prop.groupSizeExpr; }

	std::vector< std::string >
	getValues() const { return _prop.values; }

	std::vector< std::pair< std::string, std::string > > 
	getRange() const { return _prop.rangeExpr; }
	
	std::vector< std::pair< std::string, std::string > > 
	getRepetitions() const { return _prop.repetExpr; }
	
	std::map< std::string, std::string >
	getMap() const { return _prop.map; }

	EType        getType()         const { return _prop.type; }
	ESubType     getSubType()      const { return _prop.subType; }
	EDisplayType getDisplayType()  const { return _prop.displayType; }
	EStatus      getStatus()       const { return _prop.status; }
	std::string  getStringStatus() const;
	
	char* getData() const;
	void  set( const char* data, const size_t& size );

	bool isGroup()     const { return _prop.isGroup; }
	bool isOrdered()   const { return _prop.isOrdered; }
	bool isOptional()  const { return _prop.isOptional; }
	bool isBigEndian() const { return _prop.bigEndianData; }

	std::weak_ptr< Element > getParent() const { return _parent; }
	size_t getChildrenNumber() const;

	void getEndianOrderedData( char* buffer, const char* data ) const;

	void addErrorLabel( const std::string& error );
	void addWarningLabel( const std::string& warning );

	std::string getErrorLabel();
	std::string getWarningLabel();
	
	virtual void check() = 0;

protected:
	std::weak_ptr< Element >                  _parent;
	const std::weak_ptr< Element >            _previous;
	const spec_reader::SpecNode*              _specNode;
	std::vector< std::shared_ptr< Element > > _children;
	
	Properties _prop;
	bool       _checkedGroup;
	
};

}

#endif
