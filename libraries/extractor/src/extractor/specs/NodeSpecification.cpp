#include "NodeSpecification.hpp"

#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/Translator.hpp>


#include <boost/foreach.hpp>

#include <common/global.hpp>
#include <iomanip>

const std::string kId     = "id";
const std::string kLabel  = "label";

const std::string kAscii  = "ascii";
const std::string kHexa   = "hexa";
const std::string kType   = "type";
const std::string kCount  = "count";

const std::string kEndian        = "endian";
const std::string kEndianBig     = "big";
const std::string kEndianLittle  = "little";

const std::string kOptional      = "optional";
const std::string kOptionalTrue  = "true";
const std::string kOptionalFalse = "false";

NodeSpecification::NodeSpecification( File* file )
	: _file ( file )
{

}

template< typename IntType >
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

template< typename Type >
std::string getPrintable( Type value )
{
	std::ostringstream os;
	os << value;
	return os.str();
}

template< >
std::string getPrintable<uint8>( uint8 value )
{
	std::ostringstream os;
	os << (int)value;
	return os.str();
}

template< >
std::string getPrintable<int8>( int8 value )
{
	std::ostringstream os;
	os << (int)value;
	return os.str();
}


template< typename IntType >
bool isValidInt( File* _file, std::string& message, const std::string& type, const bool isBigEndian, IntType& value )
{
	if( type == getStringForType<IntType>() )
	{
		size_t size = sizeof( IntType );
		char buffer[ size ];
		_file->readData( buffer, size );

		Translator<IntType> tr;
		value = tr.translate( buffer, size, isBigEndian );

		message += " = " + getPrintable( value );
		
		return true;
	}
	return false;
}

bool NodeSpecification::isValid( SubSpec& subSpec )
{
	bool isValid = false;
	std::string message;
	std::string id         = subSpec.second.get< std::string >( kId );
	std::string label      = subSpec.second.get< std::string >( kLabel, "" );
	std::string asciiValue = subSpec.second.get< std::string >( kAscii, "" );
	std::string hexaValue  = subSpec.second.get< std::string >( kHexa, "" );
	std::string type       = subSpec.second.get< std::string >( kType, "" );
	std::string count      = subSpec.second.get< std::string >( kCount, "" );
	bool endianValue = ( subSpec.second.get<std::string>( kEndian, kEndianBig ) == kEndianBig );
	bool optional    = ( subSpec.second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );

	//COMMON_COUT( "label " << label );
	
	if( asciiValue != "" )
	{
		message += kAscii + " => " + asciiValue;
		size_t size = asciiValue.size();
		Ascii value;

		//COMMON_COUT( "read data " << size );
		char buffer[ size ];
		if( ! _file->readData( buffer, size ) )
			return optional;
		
		Translator<Ascii> tr;
		value = tr.translate( buffer, size );

		isValid = ( asciiValue ==  value.value );
		
		if( optional && !isValid )
		{
			_file->goBack( size );
			return true;
		}

		//COMMON_COUT_VAR2( asciiValue, value.value );
	}

	if( hexaValue != "" )
	{
		message += kHexa + " => " + hexaValue;
		size_t size = 0.5 * hexaValue.size() ;
		Hexa value;

		//COMMON_COUT( "read data " << size );
		char buffer[ size ];
		if( ! _file->readData( buffer, size ) )
			return optional;

		Translator<Hexa> tr;
		value = tr.translate( buffer, size );

		isValid = ( hexaValue ==  value.value );

		//COMMON_COUT_VAR2( hexaValue, value.value );
	}

	std::string typeValue = subSpec.second.get< std::string >( kType, "" );
	if( typeValue != "" )
	{
		message += kType + " => " + typeValue;

		uint8  uint8Val  = 0;
		int8   int8Val   = 0;
		uint16 uint16Val = 0;
		int16  int16Val  = 0;
		uint32 uint32Val = 0;
		int32  int32Val  = 0;
		
		bool validUInt8  = isValidInt<uint8> ( _file, message, type, endianValue, uint8Val );
		bool validInt8   = isValidInt<int8>  ( _file, message, type, endianValue, int8Val );
		bool validUInt16 = isValidInt<uint16>( _file, message, type, endianValue, uint16Val );
		bool validInt16  = isValidInt<int16> ( _file, message, type, endianValue, int16Val );
		bool validUInt32 = isValidInt<uint32>( _file, message, type, endianValue, uint32Val );
		bool validInt32  = isValidInt<int32> ( _file, message, type, endianValue, int32Val );

		bool validData = false;
		
		if( validUInt8 )
		{
			_headerElements[ id ] = uint8Val;
			//COMMON_COUT( "add id : " << id << " = " << _headerElements[ id ] );
		}
		if( validInt8 )
		{
			_headerElements[ id ] = int8Val;
		}
		if( validUInt16 )
		{
			_headerElements[ id ] = uint16Val;
		}
		if( validInt16 )
		{
			_headerElements[ id ] = int16Val;
		}
		if( validUInt32 )
		{
			_headerElements[ id ] = uint32Val;
		}
		if( validInt32 )
		{
			_headerElements[ id ] = int32Val;
		}
		
		
		if( type == "data" )
		{
			size_t size = 0;
			
			//COMMON_COUT( "research: " << count << "        " << _headerElements.size() );
			BOOST_FOREACH( ElementsPair pair, _headerElements )
			{
				//COMMON_COUT( "in " << pair.first );
				if( pair.first == count )
				{
					//COMMON_COUT( "found " << count << " = " << pair.second );
					size = pair.second;
				}
			}
			
			message += " ( " + getPrintable( size ) + " )";
			
			char buffer[ size ];
			if( ! _file->readData( buffer, size ) )
			{
				return optional;
			}
			
			validData = true;
		}
		
		isValid = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32 | validData;
	}
	
	COMMON_COUT( ( isValid ? common::details::kColorGreen : common::details::kColorRed ) << "\t" << std::left << std::setw(40) << ( label + " - " + id ) << "\t" << common::details::kColorStd << message );
	return isValid;
}
