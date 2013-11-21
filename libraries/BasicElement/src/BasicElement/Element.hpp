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

typedef std::vector< std::pair< std::string, std::string > > ExpressionList;

typedef          char       int8;
typedef unsigned char      uint8;
typedef          short      int16;
typedef unsigned short     uint16;
typedef          int        int32;
typedef unsigned int       uint32;
typedef   signed long long  int64;
typedef unsigned long long uint64;

typedef long double  ieeeExtended;    // 80 bits IEEE Standard 754 floating point

class Element
{
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	typedef std::shared_ptr< spec_reader::SpecNode  > ShPtrSpecNode;
public:
	Element( const ShPtrSpecNode node, const ShPtrElement previous = ShPtrElement(), const ShPtrElement parent = nullptr );
	
	ShPtrSpecNode next( );

	void  set( const char* data, const size_t& size );

	ShPtrElement  getParent()   const { return _parent.lock(); }
	ShPtrElement  getPrevious() const { return _previous.lock(); }
	ShPtrSpecNode getSpecNode()       { return _specNode; }

protected:
	static size_t getElementSize( const std::string& id, const EType type, const std::vector<std::string>& values );
	static size_t getElementIteration( const std::string& id, const ExpressionList& repetExpr, const ShPtrElement& previous, const ShPtrElement& parent );

protected:
	std::weak_ptr< Element >    _parent;
	std::weak_ptr< Element >    _previous;
	ShPtrSpecNode               _specNode;
	std::vector< ShPtrElement > _children;

public:
	const std::string   _id;
	const std::string   _label;
	
	const size_t        _uId;
	size_t              _size;
	const size_t        _iteration;
	size_t              _groupSize;
	
	const std::string   _countExpr;
	const std::string   _requiredExpr;
	const std::string   _groupSizeExpr;
	
	const std::vector< std::string >           _values;
	const std::map< std::string, std::string > _map;

	std::string   _mapValue;
	std::string   _dispValue;
	
	const ExpressionList _rangeExpr;
	const ExpressionList _repetExpr;

	const EType         _type;
	const EDisplayType  _displayType;
	
	EStatus       _status;
	
	std::string   _error;
	std::string   _warning;
	
	const bool    _isGroup;
	const bool    _isOrdered;
	const bool    _isOptional;
	const bool    _isBigEndian;
	bool          _checkedGroup;

	char*         _data;

};

}

#endif
