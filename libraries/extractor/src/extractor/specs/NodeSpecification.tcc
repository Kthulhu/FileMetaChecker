#include "NodeSpecification.hpp"
#include "ValuesSpecification.hpp"
#include "NumbersSpecification.hpp"

#include <extractor/inputFile/File.hpp>
#include <extractor/inputFile/Translator.hpp>
#include <extractor/expressionParser/ExpressionParser.hpp>


#include <boost/foreach.hpp>

#include <common/global.hpp>

#include <iomanip>
#include <cstdlib>
#include <cmath>

NodeSpecification::NodeSpecification( File* file )
	: _file     ( file )
{

}

	// Mettre dans une fonction "isValidSubGroup"
	// pour permettre le retour en arrière dans la spécification
	// pour afficher un erreur quand aucun subGroup n'est trouvé (ascii)
bool NodeSpecification::isValidSubGroup( SubSpec& subSpec, GroupProperties& groupProp, bpt::ptree& nodeReport, bool& ordered )
{
	bool groupIsValid = true;

	LOG_INFO( common::Color::get()->_yellow << "Start Chunk : " << subSpec.second.get< std::string >( "id" ) << common::Color::get()->_std );
	// LOG_INFO( "==> Ordered : " << ordered );
	BOOST_FOREACH( SubSpec& n, subSpec.second.get_child( kGroup ) )
	{
		bpt::ptree subNodeReport;

		if( isValid( n, groupProp, subNodeReport ) )
		{
			if( subNodeReport.size() > 0 )
			{
				nodeReport.push_back( bpt::ptree::value_type( n.second.get< std::string >( "id" ), subNodeReport ) );
			}
		}
		else
		{
			LOG_ERROR( n.second.get< std::string >( "id" ) );
			groupIsValid = false;
		}
	}
	LOG_INFO( common::Color::get()->_yellow << "End Chunk : " << subSpec.second.get< std::string >( "id" ) << common::Color::get()->_std );
	
	return groupIsValid;
}


bool NodeSpecification::isValid( SubSpec& subSpec, GroupProperties& groupProperties, bpt::ptree& nodeReport )
{
	try
	{
		std::string message( "" );
		std::string id          = subSpec.second.get< std::string >( kId );
		std::string label       = subSpec.second.get< std::string >( kLabel, "" );
		std::string typeValue   = subSpec.second.get< std::string >( kType, "" );
		std::string count       = subSpec.second.get< std::string >( kCount, "" );
		std::string groupSize   = subSpec.second.get< std::string >( kGroupSize, "" );
		
		std::vector< std::string > asciiValues = getMultipleValues< std::string >( subSpec, kAscii );
		std::vector< std::string > hexaValues  = getMultipleValues< std::string >( subSpec, kHexa  );

		bool endianValue = ( subSpec.second.get<std::string>( kEndian,   kEndianBig     ) == kEndianBig    );
		bool optional    = ( subSpec.second.get<std::string>( kOptional, kOptionalFalse ) == kOptionalTrue );
		bool ordered     = ( subSpec.second.get<std::string>( kOrdered,  kOrderedTrue   ) == kOrderedTrue  );

		bool group       = subSpec.second.get_child_optional( kGroup );
		bool isValidNode = false;
		// TLOG( "label " << label );
		
		if( asciiValues.size() != 0 )
		{
			message += kAscii + " => ";

			size_t size = asciiValues[0].size();
			for( size_t i = 0; i < asciiValues.size(); i++ )
			{
				Ascii value;

				if( i > 0 )
				{
					_file->goBack( size );
				}

				//TLOG( "read data " << size );
				char buffer[ size ];
				if( ! _file->readData( buffer, size ) )
					return optional;
				
				Translator<Ascii> tr;
				value = tr.translate( buffer, size );
				// LOG_INFO( "==> value : " << value.lowCaseValue << ", " << value.upCaseValue << " | ==> asciiValues[" << i << "] : " << asciiValues[i] );

				if( asciiValues[i] ==  value.lowCaseValue || asciiValues[i] == value.upCaseValue )
				{
					isValidNode = true;
					message += asciiValues[i];

					nodeReport.put( "<xmlattr>.optional", optional );
					nodeReport.put_value( asciiValues[i] );
					nodeReport.put( "<xmlattr>.type", "ascii" );
				}
				// TLOG_VAR2( asciiValues[i], value.value );
			}
			
			if( optional && !isValidNode )
			{
				_file->goBack( size );
				return true;
			}
			groupProperties.addSize( size );
		}

		if( hexaValues.size() != 0 )
		{
			message += kHexa + " => ";
			size_t size = 0.5 * hexaValues[0].size() ;
			
			for( size_t i = 0; i < hexaValues.size(); i++ )
			{
				Hexa value;

				if( i > 0 )
				{
					_file->goBack( size );
				}

				//TLOG( "read data " << size );
				char buffer[ size ];
				if( ! _file->readData( buffer, size ) )
					return optional;

				Translator<Hexa> tr;
				value = tr.translate( buffer, size );

				if( hexaValues[i] ==  value.value )
				{
					isValidNode = true;
					message += hexaValues[i];

					nodeReport.put( "<xmlattr>.optional", optional );
					nodeReport.put_value( hexaValues[i] );
					nodeReport.put( "<xmlattr>.type", "hexa" );
				}
				// TLOG_VAR2( hexaValues[i], value.value );
			}
			groupProperties.addSize( size );
		}

		if( !typeValue.empty() )
		{
			message += kType + " => " + typeValue;

			uint8  uint8Val  = 0;
			int8   int8Val   = 0;
			uint16 uint16Val = 0;
			int16  int16Val  = 0;
			uint32 uint32Val = 0;
			int32  int32Val  = 0;
			uint64 uint64Val = 0;
			int64  int64Val  = 0;

			float  floatVal  = 0;
			double doubleVal = 0;
			
			bool validUInt8  = isValidNumber<uint8> ( _file, message, typeValue, endianValue, subSpec, uint8Val );
			bool validInt8   = isValidNumber<int8>  ( _file, message, typeValue, endianValue, subSpec, int8Val );
			bool validUInt16 = isValidNumber<uint16>( _file, message, typeValue, endianValue, subSpec, uint16Val );
			bool validInt16  = isValidNumber<int16> ( _file, message, typeValue, endianValue, subSpec, int16Val );
			bool validUInt32 = isValidNumber<uint32>( _file, message, typeValue, endianValue, subSpec, uint32Val );
			bool validInt32  = isValidNumber<int32> ( _file, message, typeValue, endianValue, subSpec, int32Val );
			bool validUInt64 = isValidNumber<uint64>( _file, message, typeValue, endianValue, subSpec, uint64Val );
			bool validInt64  = isValidNumber<int64> ( _file, message, typeValue, endianValue, subSpec, int64Val );

			bool validFloat  = isValidNumber<float> ( _file, message, typeValue, endianValue, subSpec, floatVal );
			bool validDouble = isValidNumber<double>( _file, message, typeValue, endianValue, subSpec, doubleVal );

			bool validData   = false;
			
			exportValidData( validUInt8,  id, uint8Val,  groupProperties, nodeReport );
			exportValidData( validInt8,   id, int8Val,   groupProperties, nodeReport );
			exportValidData( validUInt16, id, uint16Val, groupProperties, nodeReport );
			exportValidData( validInt16,  id, int16Val,  groupProperties, nodeReport );
			exportValidData( validUInt32, id, uint32Val, groupProperties, nodeReport );
			exportValidData( validInt32,  id, int32Val,  groupProperties, nodeReport );
			exportValidData( validUInt64, id, uint64Val, groupProperties, nodeReport );
			exportValidData( validInt64,  id, int64Val,  groupProperties, nodeReport );
			exportValidData( validFloat,  id, floatVal,  groupProperties, nodeReport );
			exportValidData( validDouble, id, doubleVal, groupProperties, nodeReport );
			
			if( typeValue == "data" )
			{
				size_t size = 0;
				
				ExpressionParser ep = ExpressionParser();
				ep.setVariables( _headerElements );

				nodeReport.clear();

				if( count != "" )
				{
					size = ep.parseExpression<size_t>( count );
				}
				
				message += " ( size = " + getPrintable( size ) + " )";
				
				// TLOG( "*** Before error ***" );
				// char buffer[ size ];
				// TLOG( "*** After error ***" );

				// if( ! _file->readData( buffer, size ) )
				// {
				// 	return optional;
				// }

				_file->goForward( size );
				
				validData = true;
				groupProperties.addSize( size );
				nodeReport.put( "<xmlattr>.optional", optional );
				nodeReport.put( "<xmlattr>.size", getPrintable( size ) );
				nodeReport.put( "<xmlattr>.type", "data" );
			}
			
			isValidNode = validUInt8 | validInt8 | validUInt16 | validInt16 | validUInt32 | validInt32 | validUInt64 | validInt64 | validFloat | validDouble | validData;
		}

		if( group && ( isValidNode || ( ! isValidNode && asciiValues.empty() && hexaValues.empty() && typeValue.empty() ) ) )
		{
			GroupProperties groupProp;
			bool groupIsValid = isValidSubGroup( subSpec, groupProp, nodeReport, ordered );
			// LOG_INFO( ">>> " <<  subSpec.second.get< std::string >( "id" ) << ": groupIsValid : " <<  groupIsValid );
			if( !groupIsValid )
			{
				isValidNode = false;
			}
			if( groupIsValid && asciiValues.empty() && hexaValues.empty() && typeValue.empty() )
			{
				isValidNode = true;
			}
			// LOG_INFO( ">>> " <<  subSpec.second.get< std::string >( "id" ) << ": isValidNode : " <<  isValidNode );
			_file->goBack( groupProp.getSize() );

			ExpressionParser groupLength = ExpressionParser();
			groupLength.setVariables( _headerElements );
			
			if( !groupSize.empty() )
			{
				size_t gSize = groupLength.parseExpression<size_t>( groupSize );
				_file->goForward( gSize );

				if( groupProp.getSize() < gSize )
				{
					LOG_WARNING( gSize - groupProp.getSize() << " unused bytes" );
				}
				if( groupProp.getSize() > gSize )
				{
					isValidNode = false;
					LOG_ERROR( groupProp.getSize() - gSize << " bytes difference" );
				}
			}
		}
		
		if( typeValue.empty() && asciiValues.empty() && hexaValues.empty() && !group )
		{
			throw std::runtime_error( "Invalid tree : no value, group nor type node (" + id + ")" );
		}
		
		if( isValidNode )
		{
			LOG_INFO( common::Color::get()->_green << ( label + " - " + id ) << "\t" << message << common::Color::get()->_std );
		}
		else
		{
			LOG_ERROR( ( label + " - " + id ) << "\t" << message );
		}
	
		// nodeReport.put( "<xmlattr>.id", id );
		nodeReport.put( "<xmlattr>.label", label );
		nodeReport.put( "<xmlattr>.status", ( isValidNode ? "valid" : "invalid" ) );
		return isValidNode;

	}
	catch( const bpt::ptree_bad_path& pbp )
	{
		LOG_ERROR( "Invalid node in specification file : " << pbp.what() );
		throw;
	}
	catch( const bpt::ptree_bad_data& pbd )
	{
		LOG_ERROR( "Invalid data in specification node: " << pbd.what() );
		throw;
	}
	catch( const bpt::ptree_error& pe )
	{
		LOG_ERROR( "Specification file error : " << pe.what() );
		throw;
	}
	catch( std::runtime_error& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
	catch( std::exception& e )
	{
		LOG_ERROR( e.what() );
		throw;
	}
	catch(...)
	{
		LOG_ERROR( "Unknown error" );
		throw;
	}
}


template< typename DataType >
void NodeSpecification::exportValidData( const bool isValid, const std::string& id, const DataType& data, GroupProperties& groupProperties, bpt::ptree& nodeReport )
{
	if( !isValid )
		return;
	Translator<DataType> tr;
	_headerElements[ id ] = data;
	groupProperties.addSize( sizeof( DataType ) );
	nodeReport.put_value( tr.translate( data ) );
	nodeReport.put( "<xmlattr>.type", getStringForType<DataType>() );
}
