
BOOST_AUTO_TEST_SUITE( element_checker_test_checker_requirement )

BOOST_AUTO_TEST_CASE( element_checker_test_checker_requirement )
{
	std::string jsonStringFirst = R"*(
			{
				"header": [
					{ "id": "value1", "label": "Value1",
		)*";
	std::string jsonStringSecond = R"*(
					},
					{ "id": "value2", "label": "Value2", "type": "ascii", "values": "WAVE2", 
					  "required":
		)*";
	std::string jsonStringEnd = R"*(
					},
					{ "id": "value3", "label": "Value3", "type": "ascii", "values": "WAVE3" }
				]
			}
		)*";

	const char buff1[5] { 'W', 'A', 'V', 'E', '1' };
	const char buff2[5] { 'W', 'A', 'V', 'E', '2' };
	const char buff3[5] { 'W', 'A', 'V', 'E', '3' };
	const char buff4[4] { 0x00, 0x00, 0x00, 0x01 };


	LOG_INFO( "\n>>> element_checker_test_checker_requirement 1 <<<" );
	{
		std::string jsonStringFirstType         = R"*(  "type": "ascii", "values": "WAVE1" )*";
		std::string jsonStringSecondRequirement = R"*( "value1" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringFirst +
			                jsonStringFirstType +
			                jsonStringSecond +
			                jsonStringSecondRequirement +
			                jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_requiredExpr.empty() );
		elem1->set( (const char*)&buff1, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_requiredExpr.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_requiredExpr.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_requirement 2 <<<" );
	{
		std::string jsonStringFirstType         = R"*(  "type": "uint32" )*";
		std::string jsonStringSecondRequirement = R"*( "value1" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringFirst +
			                jsonStringFirstType +
			                jsonStringSecond +
			                jsonStringSecondRequirement +
			                jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_requiredExpr.empty() );
		elem1->set( (const char*)&buff4, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_requiredExpr.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_requiredExpr.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_requirement 3 <<<" );
	{
		std::string jsonStringFirstType         = R"*(  "type": "uint32" )*";
		std::string jsonStringSecondRequirement = R"*( "value1 == 1" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringFirst +
			                jsonStringFirstType +
			                jsonStringSecond +
			                jsonStringSecondRequirement +
			                jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_requiredExpr.empty() );
		elem1->set( (const char*)&buff4, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_requiredExpr.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusValid );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_requiredExpr.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
	LOG_INFO( "\n>>> element_checker_test_checker_requirement 4 <<<" );
	{
		std::string jsonStringFirstType         = R"*(  "type": "uint32" )*";
		std::string jsonStringSecondRequirement = R"*( "value1 != 1" )*";

		spec_reader::Specification spec;
		spec.setFromString( jsonStringFirst +
			                jsonStringFirstType +
			                jsonStringSecond +
			                jsonStringSecondRequirement +
			                jsonStringEnd );
		std::shared_ptr< spec_reader::SpecNode > node = spec.getFirstNode();
		BOOST_CHECK_EQUAL( node->getId(),                                       "value1"  );
		BOOST_CHECK_EQUAL( node->next()->getId(),                               "value2"  );
		BOOST_CHECK_EQUAL( node->next()->next()->getId(),                       "value3"  );
		BOOST_CHECK( node->next()->next()->next() == nullptr );

		Checker checker;
	
		std::shared_ptr< basic_element::Element > elem1( new basic_element::Element( node ) );
		BOOST_CHECK_EQUAL( elem1->_id, node->getId() );
		BOOST_CHECK( elem1->_requiredExpr.empty() );
		elem1->set( (const char*)&buff4, checker.getSize( elem1 ) );
		checker.check( elem1 );
		BOOST_CHECK_EQUAL( elem1->_status, eStatusPassOver );

		std::shared_ptr< basic_element::Element > elem2( new basic_element::Element( elem1->next(), elem1 ) );
		BOOST_CHECK_EQUAL( elem2->_id, node->next()->getId() );
		BOOST_CHECK( ! elem2->_requiredExpr.empty() );
		elem2->set( (const char*)&buff2, checker.getSize( elem2 ) );
		checker.check( elem2 );
		BOOST_CHECK_EQUAL( elem2->_isOptional, true );
		BOOST_CHECK_EQUAL( elem2->_status, eStatusSkip );

		std::shared_ptr< basic_element::Element > elem3( new basic_element::Element( elem2->next(), elem2 ) );
		BOOST_CHECK_EQUAL( elem3->_id, node->next()->next()->getId() );
		BOOST_CHECK( elem3->_requiredExpr.empty() );
		elem3->set( (const char*)&buff3, checker.getSize( elem3 ) );
		checker.check( elem3 );
		BOOST_CHECK_EQUAL( elem3->_status, eStatusValid );

		BOOST_CHECK( elem3->next() == nullptr );
	}
}

BOOST_AUTO_TEST_SUITE_END()
