#include <BasicElement/SubElements/Number.hpp>
#include <BasicElement/SubElements/Data.hpp>

BOOST_AUTO_TEST_SUITE( element_checker_test_checker )

BOOST_AUTO_TEST_CASE( element_checker_checker_data )
{
	LOG_INFO( ">>> element_checker_checker_data <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "ascii"
			)*";

	std::string jsonStringEnd = R"*(
						}
					]
				}
			)*";

	{
		bpt::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonStringBegin + jsonStringEnd );
		
		std::shared_ptr< basic_element::Data > elem = std::make_shared< basic_element::Data >( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->getStatus(), eStatusPassOver );
	}
	{
		bpt::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonStringBegin + R"*( ,"values": "WAVE" )*" + jsonStringEnd );

		std::shared_ptr< basic_element::Data > elem = std::make_shared< basic_element::Data >( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->getStatus(), eStatusValid );
	}
	{
		bpt::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonStringBegin + R"*( ,"values": "wave" )*" + jsonStringEnd );

		std::shared_ptr< basic_element::Data > elem = std::make_shared< basic_element::Data >( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->getStatus(), eStatusValid );
	}
	{
		bpt::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonStringBegin + R"*( ,"values": "evaw" )*"  + jsonStringEnd );

		std::shared_ptr< basic_element::Data > elem = std::make_shared< basic_element::Data >( &node );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->getStatus(), eStatusInvalid );
		BOOST_CHECK_EQUAL( elem->getErrorLabel(), "Invalid value" );
	}
}

BOOST_AUTO_TEST_CASE( element_checker_checker_number )
{
	LOG_INFO( ">>> element_checker_checker_number <<<" );
	
	std::string jsonStringBegin = R"*(
				{
					"header": [
						{ 
							"id": "id",
							"label": "label",
							"type": "uint32"
			)*";

	std::string jsonStringEnd = R"*(
						}
					]
				}
			)*";

	{
		bpt::ptree tree;
		spec_reader::SpecNode node = getSpecNode( tree, jsonStringBegin + jsonStringEnd );
		
		std::shared_ptr< basic_element::Number > elem = std::make_shared< basic_element::Number >( &node );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x01 };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->getStatus(), eStatusPassOver );
	}
}

BOOST_AUTO_TEST_SUITE_END()
