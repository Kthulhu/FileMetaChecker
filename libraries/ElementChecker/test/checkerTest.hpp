
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
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "WAVE" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "wave" )*" + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}
	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"values": "evaw" )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 'W', 'A', 'V', 'E' };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
		BOOST_CHECK_EQUAL( elem->_error, "Invalid value " );
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
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x01 };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusPassOver );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x05 };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"endian":"little","range": [ { "min": "1", "max": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x05, 0x00, 0x00, 0x00 };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusValid );
	}

	{
		spec_reader::Specification spec;
		spec.setFromString( jsonStringBegin + R"*( ,"range": [ { "max": "5" }, { "min": "10" } ] )*"  + jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();

		std::shared_ptr< basic_element::Element > elem( new basic_element::Element( node ) );
	
		const char buff[4] { 0x00, 0x00, 0x00, 0x08 };
		elem->set( (const char*)&buff, 4 );

		Checker( elem ).check();
		BOOST_CHECK_EQUAL( elem->_type, eTypeUInt32 );
		BOOST_CHECK_EQUAL( elem->_status, eStatusInvalid );
	}
}

BOOST_AUTO_TEST_SUITE_END()
