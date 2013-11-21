#ifndef _ELEMENT_CHECKER_CHECKER_HPP_
#define _ELEMENT_CHECKER_CHECKER_HPP_

#include <Common/common.hpp>
#include <BasicElement/Element.hpp>

#include <iostream>
#include <memory>

namespace element_checker
{

class Checker
{
	typedef std::shared_ptr< basic_element::Element > ShPtrElement;
	
public:
	Checker();

	void   check  ( const ShPtrElement element );
	size_t getSize( const ShPtrElement element );
	std::vector< ShPtrElement > getElementList() { return _elementList; }
private:
	bool isIterationValid         ( const ShPtrElement element, std::string& errorMessage );
	bool isRequirementValid       ( const ShPtrElement element );
	void checkLastUnorderedElement( const ShPtrElement element );

private:
	std::vector< ShPtrElement > _elementList;
};

}

#endif
