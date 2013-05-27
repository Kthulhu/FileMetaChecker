
#include "common.hpp"
#include "convertersTools.hpp"

#include <extractor/inputFile/File.hpp>

#include <boost/foreach.hpp>

template< typename NumberType >
std::string getStringForType()
{
	return "";
}

template< >
std::string getStringForType<uint8>()
{
	return "uint8";
}

template< >
std::string getStringForType<int8>()
{
	return "int8";
}

template< >
std::string getStringForType<uint16>()
{
	return "uint16";
}

template< >
std::string getStringForType<int16>()
{
	return "int16";
}

template< >
std::string getStringForType<uint32>()
{
	return "uint32";
}

template< >
std::string getStringForType<int32>()
{
	return "int32";
}

template< >
std::string getStringForType<uint64>()
{
	return "uint64";
}

template< >
std::string getStringForType<int64>()
{
	return "int64";
}

template< >
std::string getStringForType<float>()
{
	return "float";
}

template< >
std::string getStringForType<double>()
{
	return "double";
}

template< typename NumberType >
bool getRange( SubSpec& subSpec, const NumberType value )
{
	bool isInRange = true;
	if( boost::optional< const Spec& > rangeNode = subSpec.second.get_child_optional( kRange ) )
	{
		isInRange = false;
		BOOST_FOREACH( SubSpec& m, rangeNode.get() )
		{
			if( m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{
				NumberType max = m.second.get< NumberType >( kMax );
				NumberType min = m.second.get< NumberType >( kMin );
				if( value >= min && value <= max )
				{
					isInRange = true;
				}
			}
			if( !m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{			
				NumberType max = m.second.get< NumberType >( kMax );
				if( value <= max )
				{
					isInRange = true;
				}
			}
			if( m.second.get_child_optional( kMin ) && !m.second.get_child_optional( kMax ) )
			{			
				NumberType min = m.second.get< NumberType >( kMin );
				if( value >= min )
				{
					isInRange = true;
				}
			}
		}
	}
	if(!isInRange)
	{
		LOG_ERROR( "Value error : out of range" );
	}
	return isInRange;
}

template< >
bool getRange( SubSpec& subSpec, const int8 value )
{
	bool isInRange = true;
	if( boost::optional< const Spec& > rangeNode = subSpec.second.get_child_optional( kRange ) )
	{
		isInRange = false;
		BOOST_FOREACH( SubSpec& m, rangeNode.get() )
		{
			if( m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{
				short max = m.second.get< short >( kMax );
				short min = m.second.get< short >( kMin );
				if( value >= min && value <= max )
				{
					isInRange = true;
				}
			}
			if( !m.second.get_child_optional( kMin ) && m.second.get_child_optional( kMax ) )
			{			
				short max = m.second.get< short >( kMax );
				if( value <= max )
				{
					isInRange = true;
				}
			}
			if( m.second.get_child_optional( kMin ) && !m.second.get_child_optional( kMax ) )
			{			
				short min = m.second.get< short >( kMin );
				if( value >= min )
				{
					isInRange = true;
				}
			}
		}
	}
	if(!isInRange)
	{
		LOG_ERROR( "Value error : out of range" );
	}
	return isInRange;
}

template< typename NumberType >
bool isValidNumber( File* _file, std::string& message, const std::string& type, const bool isBigEndian, SubSpec& subSpec, NumberType& value, std::string& mapStr )
{
	if( type == getStringForType<NumberType>() )
	{
		size_t size = sizeof( NumberType );
		char buffer[ size ];
		Translator<NumberType> tr;
		std::map< NumberType, std::string > map;
		
		_file->readData( buffer, size );
		value = tr.translate( buffer, size, isBigEndian );
		mapStr = getMap( subSpec, map, size, isBigEndian, value );
		message += " = ";

		if( mapStr.empty() )
			message += getPrintable( value );
		else
			message += mapStr + " (" + getPrintable( value ) + ")";

		return getRange( subSpec, value );
	}
	return false;
}


