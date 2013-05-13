#include "Report.hpp"
#include "Export.hpp"

#include <common/global.hpp>

#include <boost/foreach.hpp>

static const std::string kRoot = "root";

Report::Report()
{
	reportTree.put( kRoot, "" );
}

void Report::add( const std::string& entry, const std::string& value )
{
	reportTree.get_child( kRoot ).put( entry, value );
}

void Report::add( const bpt::ptree& tree, const std::string& rootpath )
{
	reportTree.get_child( kRoot ).push_back( bpt::ptree::value_type( rootpath, tree ) );
}

void Report::add( const std::vector< bpt::ptree >& tree, const std::string& rootpath )
{
	BOOST_FOREACH( bpt::ptree specTree, tree )
	{
		reportTree.get_child( kRoot ).push_back( bpt::ptree::value_type( rootpath, specTree ) );
	}
}

void Report::exportReport( const std::string& filename )
{
	Export e;
	e.writeXml( reportTree, filename );
}
