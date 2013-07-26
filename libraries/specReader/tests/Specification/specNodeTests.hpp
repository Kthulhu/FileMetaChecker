#include <boost/foreach.hpp>
#include <map>

BOOST_AUTO_TEST_SUITE( spec_reader_test_specNode )

BOOST_AUTO_TEST_CASE( spec_reader_specNode )
{
	LOG_INFO( "\n>>> spec_reader_specNode <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"id\", \"label\": \"label\", \"type\": \"type\",";
		jsonString += "   \"displayType\": \"display\", \"count\": \"123\", \"required\": true,";
		jsonString += "   \"groupSize\": \"size\", \"group\": [ \"123\" ] }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getId(),          "id"      );
		BOOST_CHECK_EQUAL( node.getLabel(),       "label"   );
		BOOST_CHECK_EQUAL( node.getType(),        "type"    );
		BOOST_CHECK_EQUAL( node.getDisplayType(), "display" );
		BOOST_CHECK_EQUAL( node.getCount(),       "123"     );
		BOOST_CHECK_EQUAL( node.getRequired(),    "true"    );
		BOOST_CHECK_EQUAL( node.hasGroup(),       true      );
		BOOST_CHECK_EQUAL( node.getGroupSize(),   "size"    );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += "  { \"key\": \"value\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 0 );
		BOOST_CHECK_THROW( node.getId(),    std::runtime_error );
		BOOST_CHECK_THROW( node.getLabel(), std::runtime_error );
		BOOST_CHECK_THROW( node.getType(),  std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_values )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_values <<<" );
	{

		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"values\": \"value\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getValues().at(0), "value" );
		BOOST_CHECK_EQUAL( node.getValues().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"values\": [ \"value1\", \"value2\", \"value3\" ] }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getValues().at(0), "value1" );
		BOOST_CHECK_EQUAL( node.getValues().at(1), "value2" );
		BOOST_CHECK_EQUAL( node.getValues().at(2), "value3" );
		BOOST_CHECK_EQUAL( node.getValues().size(), 3 );
	}
	
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_range )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_range <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" },";
		jsonString += " { \"min\": \"value3\", \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value3" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"max\": \"value1\" },";
		jsonString += " { \"min\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 2 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"range\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " { \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRange().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRange().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRange().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRange().at(2).first,  "" );
		BOOST_CHECK_EQUAL( node.getRange().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRange().size(), 3 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_repetition )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_repetition <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": 5 } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "5" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "5" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": \"value\" } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 1 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\", \"max\": \"value2\" },";
		jsonString += " { \"min\": \"value3\", \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value3" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 2 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"max\": \"value1\" },";
		jsonString += " { \"min\": \"value2\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 2 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " { \"max\": \"value4\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 3 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " \"value4\"";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 3 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"repeated\": [ ";
		jsonString += " { \"min\": \"value1\" },";
		jsonString += " { \"min\": \"value2\", \"max\": \"value3\" },";
		jsonString += " \"value4\",";
		jsonString += " 23";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).first,  "value1" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(0).second, "" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).first,  "value2" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(1).second, "value3" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).first,  "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(2).second, "value4" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(3).first,  "23" );
		BOOST_CHECK_EQUAL( node.getRepetition().at(3).second, "23" );
		BOOST_CHECK_EQUAL( node.getRepetition().size(), 4 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_get_map )
{
	LOG_INFO( "\n>>> spec_reader_specNode_get_map <<<" );
	{
		std::map<std::string, std::string> testMap {
			{ "index1", "value1" },
			{ "index2", "value2" },
			{ "index3", "value3" },
			{ "index4", "value4" }
		};
		
		std::string jsonString = " { \"header\": [ { \"map\": [ ";
		for( auto mapElem : testMap )
			jsonString += " { \"" + mapElem.first + "\": \"" + mapElem.second + "\" },";

		jsonString.erase( jsonString.end() - 1, jsonString.end() );
		jsonString += " ] } ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		
		for( auto mapElem : testMap )
			BOOST_CHECK_EQUAL( node.getMap()[ mapElem.first ],  mapElem.second );

		BOOST_CHECK_EQUAL( node.getMap().size(), 4 );
	}
	
	{
		std::string jsonString = " { \"header\": [ { \"map\": [ ] } ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}

	{
		std::string jsonString = " { \"header\": [ { \"unknownKey\": \"noValue\"} ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}
	
	{
		std::string jsonString = " { \"header\": [ { }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getMap().size(), 0 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_next )
{
	LOG_INFO( "\n>>> spec_reader_specNode_next <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"value1\" },";
		jsonString += " { \"id\": \"value2\" },";
		jsonString += " { \"id\": \"value3\" },";
		jsonString += " { \"id\": \"value4\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getId(),                      "value1" );
		BOOST_CHECK_EQUAL( node.next().getId(),               "value2" );
		BOOST_CHECK_EQUAL( node.next().next().getId(),        "value3" );
		BOOST_CHECK_EQUAL( node.next().next().next().getId(), "value4" );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_first_child )
{
	LOG_INFO( "\n>>> spec_reader_specNode_first_child <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"value1\",";
		jsonString += "   \"group\": [ ";
		jsonString += " { \"id\": \"value11\" },";
		jsonString += " { \"id\": \"value12\" },";
		jsonString += " { \"id\": \"value13\" }";
		jsonString += " ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getId(),  "value1" );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );

		node = node.firstChild();
		BOOST_CHECK_EQUAL( node.getId(), "value11" );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value12" );
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value13" );
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
	}
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"value1\" },";
		jsonString += " { \"id\": \"value2\" },";
		jsonString += " { \"id\": \"value3\" },";
		jsonString += " { \"id\": \"value4\" }";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 4 );
		BOOST_CHECK_EQUAL( node.getId(), "value1"  );
		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value2" );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value3" );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value4" );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      4 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 4 );
	}
}

BOOST_AUTO_TEST_CASE( spec_reader_specNode_first_child_recursivity )
{
	LOG_INFO( "\n>>> spec_reader_specNode_first_child_recursivity <<<" );
	{
		std::string jsonString = " { \"header\": [ ";
		jsonString += " { \"id\": \"value1\",";
		jsonString += "   \"group\": [ ";
		jsonString += "     { \"id\": \"value11\" },";
		jsonString += "     { \"id\": \"value12\",";
		jsonString += "       \"group\": [ ";
		jsonString += "          { \"id\": \"value121\", ";
		jsonString += "            \"group\": [ ";
		jsonString += "               { \"id\": \"value1211\" },";
		jsonString += "               { \"id\": \"value1212\" }";
		jsonString += "            ] } ";
		jsonString += "       ] }, ";
		jsonString += "     { \"id\": \"value13\" }";
		jsonString += "   ] } ";
		jsonString += " ] } ";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );
		SpecNode node( tree.get_child( "header" ).begin(), 0, 1 );
		BOOST_CHECK_EQUAL( node.getId(),  "value1" );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 1 );

		node = node.firstChild();
		BOOST_CHECK_EQUAL( node.getId(), "value11" );
		BOOST_CHECK_EQUAL( node.getIndex(),      0 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
		
		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value12" );
		BOOST_CHECK_EQUAL( node.getIndex(),      1 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		SpecNode childNode = node.firstChild();
		BOOST_CHECK_EQUAL( childNode.getId(), "value121" );
		BOOST_CHECK_EQUAL( childNode.getIndex(),       0 );
		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),  1 );

		childNode = childNode.next();
		BOOST_CHECK_EQUAL( childNode.getIndex(),      1 );
		BOOST_CHECK_EQUAL( childNode.getIndexTotal(), 1 );

		childNode = childNode.firstChild();
		BOOST_CHECK_EQUAL( childNode.getId(), "value1211" );
		BOOST_CHECK_EQUAL( childNode.getIndex(),        0 );
		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),   2 );

		childNode = childNode.next();
		BOOST_CHECK_EQUAL( childNode.getId(), "value1212" );
		BOOST_CHECK_EQUAL( childNode.getIndex(),        1 );
		BOOST_CHECK_EQUAL( childNode.getIndexTotal(),   2 );

		BOOST_CHECK_THROW( childNode.firstChild(), std::runtime_error );
		
		childNode = childNode.next();
		BOOST_CHECK_EQUAL( childNode.getIndex(),      2 );
		BOOST_CHECK_EQUAL( childNode.getIndexTotal(), 2 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getId(), "value13" );
		BOOST_CHECK_EQUAL( node.getIndex(),      2 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		BOOST_CHECK_THROW( node.firstChild(), std::runtime_error );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );

		node = node.next();
		BOOST_CHECK_EQUAL( node.getIndex(),      3 );
		BOOST_CHECK_EQUAL( node.getIndexTotal(), 3 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
