#include <BasicElement/SubElements/Number.hpp>
#include <BasicElement/SubElements/Data.hpp>
#include <ElementChecker/Translator/Translator.hpp>

using namespace element_checker;

BOOST_AUTO_TEST_SUITE( element_checker_test_translator )

BOOST_AUTO_TEST_CASE( element_checker_translator_1 )
{
	LOG_INFO( ">>> element_checker_translator_1 <<<" );
	std::string jsonString = R"*(
			{
				"header": [
					{ 
						"id": "id",
						"label": "label",
						"type": "type",
						"displayType": "display",
						"count": "123",
						"required": true,
						"groupSize": "size",
						"group": [
							"123"
						]
					}
				]
			}
		)*";

	std::istringstream isstream( jsonString );
	bpt::ptree tree;

	bpt::read_json( isstream, tree );

	spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
	basic_element::Number elem( &node );

	{
		Translator tr = Translator( &elem );
		BOOST_CHECK_THROW( tr.get< std::string >(), std::runtime_error );
	}
	{
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	}
	{
		const char buff[2] { 'a', 'b' };
		elem.set( (const char*)&buff, 2 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "ab" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< short > >().at(0), 25185 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 97 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 98 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< short >(), 25185 );
		BOOST_CHECK_THROW( Translator( &elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get< std::vector< int > >(), std::runtime_error );
	}
	{
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		basic_element::Data data( &node );
		const char buff[4] { 'W', 'A', 'V', 'E' };
		data.set( (const char*)&buff, 4 );

		BOOST_CHECK_EQUAL( Translator( &data ).get( eDisplayTypeDefault ), "" );
		BOOST_CHECK_EQUAL( Translator( &data ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &data ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &data ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &data ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
}

BOOST_AUTO_TEST_CASE( element_checker_translator_2 )
{
	LOG_INFO( ">>> element_checker_translator_2 <<<" );
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "ascii"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		basic_element::Data elem( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeDefault ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		basic_element::Number elem( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem.set( (const char*)&buff, 4 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::vector< int > >().at(0), 1163280727 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(1), 65 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(2), 86 );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(3), 69 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<   int >(), 1163280727 );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeDefault ), "1163280727" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeNumbers ), "87658669" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeAscii   ), "WAVE" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeHexa    ), "57415645" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeRaw     ), "WAVE" );
		
	}
	{
		std::string jsonString = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "int8"
						}
					]
				}
			)*";

		std::istringstream isstream( jsonString );
		bpt::ptree tree;

		bpt::read_json( isstream, tree );

		spec_reader::SpecNode node( tree.get_child( "header" ).begin() );
		basic_element::Number elem( &node );
	
		const char buff[1] { 'W' };
		elem.set( (const char*)&buff, 1 );


		BOOST_CHECK_EQUAL( Translator( &elem ).get< std::string >(), "W" );
		BOOST_CHECK_EQUAL( (short) Translator( &elem ).get< std::vector< char > >().at(0), 87 );
		BOOST_CHECK_EQUAL( Translator( &elem ).get<  char >(), 'W' );
		BOOST_CHECK_THROW( Translator( &elem ).get< std::vector< int > >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get<   int >(), std::runtime_error );
		BOOST_CHECK_THROW( Translator( &elem ).get< short >(), std::runtime_error );
	
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeDefault ), "87" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeNumbers ), "87" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeAscii   ), "W" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeHexa    ), "57" );
		BOOST_CHECK_EQUAL( Translator( &elem ).get( eDisplayTypeRaw     ), "W" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
