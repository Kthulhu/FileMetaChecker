#include "Data.hpp"

#include <iostream>

namespace basic_element
{
namespace data_element
{

Data::Data()
	: Element( eTypeData )
	, _data( NULL )
{
	_subType = eDataTypeRaw;
	_size = 0;
}

Data::Data( const EDataType& subType )
	: Element( eTypeData )
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
	if( _data == NULL )
		return "";

	std::stringstream sstr;
	for (size_t i = 0; i < _size; ++i)
		sstr << _data[i];
	//BE_LOG_TRACE( " Data: \tTO STRING (Ascii): " << sstr.str() );
	return sstr.str();
}

std::string Data::getHexa() const
{
	if( _data == NULL )
		return "";

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
	_size = _specValue.size();
}

void Data::setSpecData( const std::vector< std::string >& specValues )
{
	_specValues = specValues;
	size_t size = 0;
	for( std::string value: _specValues )
	{
		if( size != 0 && size != value.size() )
			throw std::runtime_error( "Specification Error: Multiple values must have the same size" );
		size = value.size();
	}
	_size = size;
}

Element::EStatus Data::checkData()
{
	if( _specValue.empty() && _specValues.empty() )
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
			if( ! _specValue.empty() && _specValue == getAscii() )
				status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == getAscii() )
						status = eStatusValid;
			}
			break;
		}

		case eDataTypeHexa :
		{
			if( ! _specValue.empty() && _specValue == getHexa()  )
				status = eStatusValid;
			if( ! _specValues.empty() )
			{
				for( std::string value : _specValues )
					if( value == getHexa() )
						status = eStatusValid;
			}
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
	std::vector< std::pair< std::string, std::string > > commonInfo = getCommonElementInfo();
	
	elemInfo.insert( elemInfo.end(), commonInfo.begin(), commonInfo.end() );

	elemInfo.push_back( std::make_pair( "type", getStringFromType() ) );
	elemInfo.push_back( std::make_pair( "data", getStringFromData() ) );
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

std::string Data::getStringFromType()
{
	std::string type;
	switch( getDataSubType() )
	{
		case eStatusUnknown : type = "unknown"; break;
		case eDataTypeAscii : type = "ascii";   break;
		case eDataTypeHexa  : type = "hexa";    break;
		case eDataTypeRaw   : type = "raw";     break;
	}
	return type;
}

std::string Data::getStringFromData()
{
	std::string data;
	switch( getDataSubType() )
	{
		case eStatusUnknown : data = "";         break;
		case eDataTypeAscii : data = getAscii(); break;
		case eDataTypeHexa  : data = getHexa();  break;
		case eDataTypeRaw   : data = "";         break;
	}
	return data;
}

char* Data::getDataPtr( ) const
{
	return _data;
}

}
}
