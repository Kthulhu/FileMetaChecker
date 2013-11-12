#include "Report.hpp"

#include <iostream>

#include <BasicElement/Element.hpp>

#include <Common/color.hpp>

namespace report_generator 
{

void Report::init( const std::vector< std::shared_ptr< basic_element::Element > >& elementList )
{
	_elementList = elementList;
}

void Report::print()
{
	std::cout << std::setfill( '-' ) << std::setw( 191 ) << " " << std::endl;
	std::cout << "|" << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Elements"  << std::setfill( ' ' ) << std::setw( 16 ) << "|"
					 << std::setfill( ' ' ) << std::setw( 16 ) << "" << "Value"     << std::setfill( ' ' ) << std::setw( 16 ) << "|"
	        		 << std::setfill( ' ' ) << std::setw(  2 ) << "" << "Iteration" << std::setfill( ' ' ) << std::setw(  2 ) << "|"
	        		 << std::setfill( ' ' ) << std::setw( 11 ) << "" << "Status"    << std::setfill( ' ' ) << std::setw( 11 ) << "|"
	        		 << std::setfill( ' ' ) << std::setw( 10 ) << "" << "Error"     << std::setfill( ' ' ) << std::setw( 11 ) << "|"
	        		 << std::setfill( ' ' ) << std::setw( 10 ) << "" << "Warning"   << std::setfill( ' ' ) << std::setw( 11 ) << "|"
	        		 << std::setfill( ' ' ) << std::setw( 10 ) << "" << "Comment"   << std::setfill( ' ' ) << std::setw( 10 ) << "|" << std::endl;
	std::cout << std::setfill( '-' ) << std::setw( 191 ) << " " << std::endl;

	for( std::shared_ptr< basic_element::Element > element : _elementList )
		print( element );
}

void Report::print( const std::shared_ptr< basic_element::Element > element )
{
	size_t count = 0;
	std::shared_ptr< basic_element::Element > elemCopy( element );
	while( 1 )
	{
		count++;
		std::weak_ptr< basic_element::Element > parent = elemCopy->getParent();
		if( parent.use_count() == 0 )
			break;
		elemCopy = parent.lock();
	}

	std::string dispColor;
	switch( element->_status )
	{
		case eStatusNotChecked               : dispColor = common::details::kColorYellow; break;
		case eStatusValid                    : dispColor = common::details::kColorGreen;  break;
		case eStatusInvalid                  : dispColor = common::details::kColorRed;    break;
		case eStatusInvalidButOptional       : dispColor = common::details::kColorRed;    break;
		case eStatusInvalidForUnordered      : dispColor = common::details::kColorRed;    break;
		case eStatusInvalidButSkip           : dispColor = common::details::kColorRed;    break;
		case eStatusInvalidGroupForIteration : dispColor = common::details::kColorRed;    break;
		case eStatusPassOver                 : dispColor = common::details::kColorGreen;  break;
		case eStatusSkip                     : dispColor = common::details::kColorYellow; break;
	}

	std::cout << dispColor 
			  << std::setfill( ' ' ) << std::setw( 5*count + 1 )
			  << element->_id
	          << std::setfill( ' ' ) << std::setw( 5*( 8 - count ) + 36 )
	          << element->_dispValue
	          << std::setfill( ' ' ) << std::setw( 12 )
	          << element->_iteration
	          << std::setfill( ' ' ) << std::setw( 28 )
	          << statusMap.at( element->_status )
	          << std::setfill( ' ' ) << std::setw( 26 )
	          << element->_error
	          << std::setfill( ' ' ) << std::setw( 28 )
	          << element->_warning;

	std::string comment = "";
	if( element->_isOptional )
		comment += "Optional ";
	if( element->_isGroup )
		comment += "Group ";
	if( ! element->_repetExpr.empty() )
		comment += "Repeated ";
	if( ! element->_isOrdered )
		comment += "Unordered ";

	std::cout << std::setfill( ' ' ) << std::setw( 30 )
	          << ( comment.empty() ? "- " : comment )
	          << common::details::kColorStd << std::endl;
}

}

