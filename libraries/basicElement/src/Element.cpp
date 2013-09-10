#include "Element.hpp"
#include "common.hpp"

namespace basic_element
{

size_t Element::_lastUniqueId = 0;

Element::Element( EType type )
	: _type          ( type )
	, _uniqueId      ( _lastUniqueId++ )
	, _status        ( eStatusUnknown )
	, _subType       ( 0 )
	, _error         ( "" )
	, _warning       ( "" )
	, _bigEndianData ( true )
{
}

void Element::setId( const std::string& id )
{
	_id = id;
}

void Element::setLabel( const std::string& label )
{
	_label = label;
}

ENumberType Element::getNumberSubType()
{
	if( _type != eTypeNumber )
		return eNumberTypeUnknown;
	return ( ENumberType ) _subType;
}

EDataType Element::getDataSubType()
{
	if( _type != eTypeData )
		return eDataTypeUnknown;
	return ( EDataType ) _subType;
}

void Element::setStatus( const EStatus status )
{
	_status = status;
}

void Element::setBigEndianness( bool isBigEndian )
{
	_bigEndianData = isBigEndian;
}

bool Element::getBigEndianness() const
{
	return _bigEndianData;
}

void Element::getEndianOrderedData( char* buffer, const char* data ) const
{
	if( !_bigEndianData )
	{
		//BE_LOG_TRACE( " EndianessConverter: case 1 ( little ) " );
		std::reverse_copy( data, data + _size, buffer );
	}
	else
	{
		//BE_LOG_TRACE( " EndianessConverter: case 2 ( big ) " );
		std::memcpy( buffer, data, _size );
	}
}

void Element::reverseEndianness( char* buffer, const char* data ) const
{
	std::reverse_copy( data, data + _size, buffer );
}


void Element::addErrorLabel( const std::string& error )
{
	_status = Element::eStatusInvalid;
	_error += error;
}

void Element::addWarningLabel( const std::string& warning )
{
	_warning += warning;
}

std::string Element::getErrorLabel()
{
	return _error;
}

std::string Element::getWarningLabel()
{
	return _warning;
}

std::vector< std::pair< std::string, std::string > > Element::getCommonElementInfo()
{
	std::vector< std::pair< std::string, std::string > > commonInfo;
	commonInfo.push_back( std::make_pair( "id",     getId()             ) );
	commonInfo.push_back( std::make_pair( "label",  getLabel()          ) );
	commonInfo.push_back( std::make_pair( "status", getStatusString()   ) );
	
	if( ! _error.empty() )
		commonInfo.push_back( std::make_pair( "error", _error ) );
	if( ! _warning.empty() )
		commonInfo.push_back( std::make_pair( "warning", _warning ) );

	return commonInfo;
}

std::string Element::getStatusString()
{
	std::string status;
	switch( getStatus() )
	{
		case Element::eStatusUnknown  : status = "Unknown";   break;
		case Element::eStatusValid    : status = "Valid";     break;
		case Element::eStatusInvalid  : status = "Invalid";   break;
		case Element::eStatusPassOver : status = "Pass over"; break;
	}
	return status;
}


}
