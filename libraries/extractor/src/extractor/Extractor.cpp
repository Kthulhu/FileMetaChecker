#include "Extractor.hpp"

#include <common/global.hpp>
#include <report/Report.hpp>

#include "specs/Specifications.hpp"
#include "specs/NodeSpecification.hpp"
#include "inputFile/File.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/none.hpp>
#include <boost/algorithm/string.hpp>

namespace bfs = boost::filesystem;

Extractor::Extractor()
{
	_file = new File();
	_specs = new Specifications();
}

Extractor::~Extractor()
{
	delete _file;
	delete _specs;
}

void Extractor::init()
{
	_specs->init();
	_specs->updateList();
}

bool Extractor::openFilename( const std::string& filename )
{
	bfs::path path( filename );
	_ext = path.extension().string();
	boost::algorithm::to_lower( _ext );
	return _file->open( filename );
}

void Extractor::closeFilename( )
{
	_file->close();
}

void Extractor::analyse( )
{
	Specs specs;
	_specs->getSpecList( specs );
	BOOST_FOREACH( Specification spec, specs )
	{
		std::vector< std::string > exts = spec.getsupportedExtensions();
		bool extIsSupported = false;

		COMMON_COUT( common::details::kColorBlue << "Analyse file as a : " << spec.getLabel() << common::details::kColorStd );

		BOOST_FOREACH( std::string ext, exts )
		{
			if( ext == _ext )
			{
				extIsSupported = true;
			}
		}

		if( extIsSupported )
		{
			COMMON_COUT( "-> Parse Header" );
			SpecIt header = spec.getHeader( );

			_report.put( "<xmlattr>.id", spec.getId() );
			_report.put( "<xmlattr>.label", spec.getLabel() );
			_report.put( "<xmlattr>.type", spec.getType() );
			
			NodeSpecification ns( _file );
			bool isValidFile = true;
			
			GroupProperties groupProperties;

			BOOST_FOREACH( SubSpec v, header->second )
			{
				bpt::ptree nodeReport;
				if( ! ns.isValid( v, groupProperties, nodeReport ) )
				{
					COMMON_COUT( v.second.get< std::string >( "id" ) );
					isValidFile = false;
				}
				else
				{
					_report.push_back( bpt::ptree::value_type( v.second.get< std::string >( "id" ), nodeReport ) );
				}
			}
			
			if( isValidFile )
			{
				_report.put( "<xmlattr>.status", "valid" );
				COMMON_COUT( common::details::kColorGreen );
				COMMON_COUT_X( 80, "*" );
				COMMON_COUT( "VALID " << spec.getLabel() );
				COMMON_COUT_X( 80, "*" );
				COMMON_COUT( common::details::kColorStd );
			}
			else
			{
				_report.put( "<xmlattr>.status", "not valid" );
				COMMON_COUT( common::details::kColorRed );
				COMMON_COUT_X( 80, "*" );
				COMMON_COUT( "NOT VALID " << spec.getLabel() );
				COMMON_COUT_X( 80, "*" );
				COMMON_COUT( common::details::kColorStd );
			}
		}
	}
}

void Extractor::getReport( Report* report )
{
	report->add( _report, "specification" );
}

