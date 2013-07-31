#include "Data.hpp"

#include <iostream>

namespace basic_element
{
namespace data_element
{

Data::Data()
	: Element( Element::eTypeData )
	, _data( NULL )
{
	_subType = Element::eDataTypeRaw;
	_size = 0;
}

Data::Data( const Element::EDataType& subType )
	: Element( Element::eTypeData )
	, _data( NULL )
{
	_subType = subType;
	_size = 0;
}


Data::~Data()
{
	if( _data != NULL )
	{
		delete[] _data;
		_data = NULL;
	}
}

void Data::setData( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	getEndianOrderedData( _data, data );
}

void Data::getData( char* buffer ) const
{
	//BE_LOG_TRACE( " Data: \tGET DATA from @ " << &_data << " to @ " << &buffer );
	std::memcpy( buffer, _data, _size );
}

size_t Data::getSize() const
{
	return _size;
}

std::string Data::getAscii() const
{
	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	//BE_LOG_TRACE( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

std::string Data::getHexa() const
{
	std::stringstream sstr;
	char* buffer = new char [ _size ];
	std::memcpy( buffer, _data, _size );
	for( size_t i = 0; i < _size; ++i )
	{
		sstr << std::hex << std::setfill('0') << std::setw(2) << (int)(unsigned char)buffer[i];
	}
	delete[] buffer;
	//BE_LOG_TRACE( " Data: \tTO STRING (Hexa): " << sstr.str() );
	return sstr.str();
}

std::vector< unsigned int > Data::toIntVector()
{
	std::vector< unsigned int > vector;
	for( size_t i = 0; i < _size; ++i )
	{
		vector.push_back( ( unsigned char ) _data[i] );
		//BE_LOG_TRACE( " Data: \tTO INT VECTOR : " << vector.at( i ) );
	}
	return vector;
}

void Data::setSpecData( const std::string& specValue )
{
	_specValue = specValue;
}

Element::EStatus Data::checkData()
{
	if( _specValue.empty() )
	{
		setStatus( eStatusPassOver );
		return eStatusPassOver;
	}

	Element::EStatus status = eStatusInvalid;
	switch( getDataSubType() )
	{
		case eDataTypeUnknown :
		{
			status = eStatusUnknown;
			break;
		}
		case eDataTypeAscii :
		{
			if( _specValue == getAscii() )
				status = eStatusValid;
			break;
		}

		case eDataTypeHexa :
		{
			if( _specValue == getHexa()  )
				status = eStatusValid;
			break;
		}

		case eDataTypeRaw :
		{
			status = eStatusPassOver;
			break;
		}
	}
	
	setStatus( status );
	return status;
}

std::vector< std::pair< std::string, std::string > > Data::getElementInfo()
{
	std::vector< std::pair< std::string, std::string > > elemInfo;
	std::string status;
	switch( getStatus() )
	{
		case Element::eStatusUnknown  : status = "Unknown";   break;
		case Element::eStatusValid    : status = "Valid";     break;
		case Element::eStatusInvalid  : status = "Invalid";   break;
		case Element::eStatusPassOver : status = "Pass over"; break;
	}

	std::string data;
	switch( getDataSubType() )
	{
		case Element::eStatusUnknown : data = "";         break;
		case Element::eDataTypeAscii : data = getAscii(); break;
		case Element::eDataTypeHexa  : data = getHexa();  break;
		case Element::eDataTypeRaw   : data = "";         break;
	}
	
	elemInfo.push_back( std::make_pair( "id",     getId()    ) );
	elemInfo.push_back( std::make_pair( "label",  getLabel() ) );
	elemInfo.push_back( std::make_pair( "status", status     ) );
	elemInfo.push_back( std::make_pair( "data",   data       ) );
	return elemInfo;
}

Data& Data::operator=( const Data& other )
{
	if( this != &other )
	{
		this->setData( other.getDataPtr(), other.getSize() );
	}
	return *this;
}

char* Data::getDataPtr( ) const
{
	return _data;
}

}
}
