
// contains parsing, range, map, displayType, values and count tests

BOOST_AUTO_TEST_SUITE( comparator_test_suite_features )

BOOST_AUTO_TEST_CASE( comparator_comparator )
{
	LOG_WARNING( ">>> comparator_comparator <<<" );
	// {
	// 	sr::SpecList specs;
	// 	sr::Specification spec;
	// 	spec.setFromFile( "comparetest.json" );
	// 	specs.addSpecification( spec );
	// 	BOOST_CHECK_EQUAL( specs.getSpecNumber(), 1 );
		
	// 	std::ifstream is;
	// 	is.open( "test.txt", std::ios::in );
	// 	fr::FileReader file( is.rdbuf() );

	// 	Comparator comp( &file, specs );
		
	// 	rg::Report report;
	// 	comp.compare( "test", report );
	// }
	{
		std::string jsonString =R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "unknown"
				}
			]
		})*";
		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		BOOST_CHECK_THROW( comp.compare( "test", report ), std::runtime_error );
	}
	{
		std::string jsonString =R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "ascii"
				}
			]
		})*";
		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		BOOST_CHECK_THROW( comp.compare( "test", report ), std::runtime_error );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_only_root )
{
	LOG_WARNING( ">>> comparator_comparator_only_root <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0"
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "test3" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_one_child )
{
	LOG_WARNING( ">>> comparator_comparator_one_child <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "hexa",
							"values": "ff00"
						}
					]
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "test3" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}


BOOST_AUTO_TEST_CASE( comparator_comparator_n_children )
{
	LOG_WARNING( ">>> comparator_comparator_n_children <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "hexa",
							"values": "ff00"
						},
						{
							"id": "child2",
							"label": "Child 2",
							"type": "hexa",
							"values": "00ff"
						},
						{
							"id": "child3",
							"label": "Child 3",
							"type": "hexa",
							"values": "00ff"
						},
						{
							"id": "child4",
							"label": "Child 4",
							"type": "hexa",
							"values": "00ff"
						},
						{
							"id": "child5",
							"label": "Child 5",
							"type": "hexa",
							"values": "00ff"
						}
					]
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "test3" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}


BOOST_AUTO_TEST_CASE( comparator_comparator_one_subchild )
{
	LOG_WARNING( ">>> comparator_comparator_one_subchild <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "hexa",
							"values": "ff00"
						},
						{
							"id": "child2",
							"label": "Child 2",
							"type": "hexa",
							"values": "00ff",
							"group": [
								{
									"id": "child21",
									"label": "Child 21",
									"type": "hexa",
									"values": "00ff"
								}
							]
						}						
					]
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "test3" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_n_subchildren )
{
	LOG_WARNING( ">>> comparator_comparator_n_subchildren <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "hexa",
							"values": "ff00"
						},
						{
							"id": "child2",
							"label": "Child 2",
							"type": "hexa",
							"values": "00ff",
							"group": [
								{
									"id": "child21",
									"label": "Child 21",
									"type": "hexa",
									"values": "00ff"
								},
								{
									"id": "child22",
									"label": "Child 22",
									"type": "hexa",
									"values": "00ff"
								}
							]
						}						
					]
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "test3" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_n_levels )
{
	LOG_WARNING( ">>> comparator_comparator_n_levels <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "hexa",
					"count": "0"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "hexa",
							"values": "4649"
						},
						{
							"id": "child2",
							"label": "Child 2",
							"type": "hexa",
							"values": "4c45",
							"group": [
								{
									"id": "child21",
									"label": "Child 21",
									"type": "hexa",
									"values": "2072",
									"group": [
										{
											"id": "child211",
											"label": "Child 211",
											"type": "hexa",
											"values": "6561"
										},
										{
											"id": "child212",
											"label": "Child 212",
											"type": "hexa",
											"values": "6465",
											"group": [
												{
													"id": "child2121",
													"label": "Child 2121",
													"type": "hexa",
													"values": "7220"
												},
												{
													"id": "child2122",
													"label": "Child 2122",
													"type": "hexa",
													"values": "2020",
													"group": [
														{
															"id": "child21221",
															"label": "Child 21221",
															"type": "hexa",
															"values": "2020"
														}
													]
												}
											]
										}
									]
								},
								{
									"id": "child22",
									"label": "Child 22",
									"type": "hexa",
									"values": "7265"
								}
							]
						}						
					]
				},
				{
					"id": "test3",
					"label": "Test 3",
					"type": "raw",
					"count": "0",
					"group": [
						{
							"id": "child31",
							"label": "Child 31",
							"type": "hexa",
							"values": "6461",
							"group": [
								{
									"id": "child311",
									"label": "Child 311",
									"type": "hexa",
									"values": "6572",
									"group": [
										{
											"id": "child3111",
											"label": "Child 3111",
											"type": "hexa",
											"values": "2045"
										}
									]
								}
							]
						}
					]
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader     redaer Eend" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "end" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_n_levels_2 )
{
	LOG_WARNING( ">>> comparator_comparator_n_levels_2 <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1",
				"ext2",
				"ext3"]
			},
			"header": [
				{
					"id": "test1",
					"label": "Test 1",
					"type": "uint8"
				},
				{
					"id": "test2",
					"label": "Test 2",
					"type": "ascii",
					"count": "0",
					"group": [
						{
							"id": "child1",
							"label": "Child 1",
							"type": "uint32"
						},
						{
							"id": "child2",
							"label": "Child 2",
							"type": "int8",
							"group": [
								{
									"id": "child21",
									"label": "Child 21",
									"type": "hexa",
									"values": "00ff",
									"group": [
										{
											"id": "child211",
											"label": "Child 211",
											"type": "float"
										},
										{
											"id": "child212",
											"label": "Child 212",
											"type": "double",
											"group": [
												{
													"id": "child2121",
													"label": "Child 2121",
													"type": "ieeeExtended"
												},
												{
													"id": "child2122",
													"label": "Child 2122",
													"type": "uint64",
													"group": [
														{
															"id": "child21221",
															"label": "Child 21221",
															"type": "int32"
														}
													]
												}
											]
										}
									]
								},
								{
									"id": "child22",
									"label": "Child 22",
									"type": "int16"
								}
							]
						}						
					]
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader    1.0 vs. 0.1    redaer ELIF ? end" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "end" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_1 )
{
	LOG_WARNING( ">>> comparator_comparator_validation_1 <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"]
			},
			"header": [
				{
					"id": "file",
					"label": "It is written FILE",
					"type": "ascii",
					"values": "FILE"
				},
				{
					"id": "space",
					"label": "It is a space character",
					"type": "uint8",
					"range" : [
						{ "min": 31, "max": 33 }
					]
				},
				{
					"id": "reader",
					"label": "It is a written reader",
					"type": "ascii",
					"values": "reader"
				},
				{
					"id": "number",
					"label": "It is a number",
					"type": "uint8",
					"endian": "little",
					"map" : [
						{ "32" : "Value of the number" }
					]
				},
				{
					"id": "otherNumber",
					"label": "It is an other number",
					"type": "uint8",
					"map" : [
						{ "33" : "Something" }
					],
					"range" : [
						{ "min": 31, "max": 33 }
					]
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader  end" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "end" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_CASE( comparator_comparator_validation_2 )
{
	LOG_WARNING( ">>> comparator_comparator_validation_2 <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "file",
					"label": "It is written FILE",
					"type": "ascii",
					"values": "FILE"
				},
				{
					"id": "space",
					"label": "It is a space character",
					"type": "uint8",
					"range": [
						{ "min": 31, "max": 33 }
					]
				},
				{
					"id": "reader",
					"label": "It is a written reader",
					"type": "ascii",
					"values": "reader"
				},
				{
					"id": "number",
					"label": "It is a number",
					"type": "uint8",
					"endian": "little",
					"map": [
						{ "32" : "Value of the number" }
					]
				},
				{
					"id": "hexaSpace",
					"label": "It is an other space character",
					"type": "hexa",
					"values": "20"
				},
				{
					"id": "rawData",
					"label": "There is some raw data",
					"type": "raw",
					"count": "7",
					"displayType": "ascii"
				},
				{
					"id": "hexaToAscii",
					"label": "There are hexa characters",
					"type": "hexa",
					"values": "40202e",
					"displayType": "ascii"
				},
				{
					"id": "hexaToRaw",
					"label": "There are other hexa characters",
					"type": "hexa",
					"values": "2f2c2b",
					"displayType": "raw"
				},
				{
					"id": "zeroToHexa",
					"label": "It is a zero character",
					"type": "ascii",
					"values": "0",
					"displayType": "hexa"
				},
				{
					"id": "digitsToHexa",
					"label": "There are some digit characters",
					"type": "ascii",
					"count": "5",
					"displayType": "hexa"
				},
				{
					"id": "charactersToRaw",
					"label": "There are some characters",
					"type": "ascii",
					"count": "8",
					"displayType": "raw"
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "FILE reader  nothing@ ./,+0031560?12!3#4end" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "end" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}


BOOST_AUTO_TEST_CASE( comparator_comparator_validation_count_expression )
{
	LOG_WARNING( ">>> comparator_comparator_validation_count_expression <<<" );
	{
		std::string jsonString = R"*(
		{
			"standard":
			{
				"id": "test",
				"extension": [
				"ext1"
				]
			},
			"header": [
				{
					"id": "length",
					"label": "Sixteen times length of FILE",
					"type": "uint8"
				},
				{
					"id": "file",
					"label": "FILE",
					"type": "ascii",
					"count": "length / 16"
				},
				{
					"id": "size",
					"label": "Eleven times size HEY",
					"type": "uint8"
				},
				{
					"id": "hey",
					"label": "HEY",
					"type": "ascii",
					"count": "size / 11"
				},
				{
					"id": "end",
					"label": "End",
					"type": "ascii",
					"values": "end"
				}
			]
		}
		)*";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::stringbuf buffer;
		buffer.str( "@FILE!HEYend" );
		fr::FileReader file( &buffer );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		bpt::ptree xmlTransReport = tr.transformTree( rg::Transform::eReportTypeXml );
		rg::Export exporter( xmlTransReport );
		
		LOG_INFO( "\n==== REPORT ====" );
		LOG_INFO( exporter.getXmlString() );

		BOOST_CHECK_EQUAL( xmlTransReport.end()->second.data(), "end" );

		std::istringstream  xmlStream( exporter.getXmlString() );
		std::istringstream jsonStream( jsonString );
		bpt::ptree  xmlReport;
		bpt::ptree jsonReport;
		bpt::read_xml (  xmlStream,  xmlReport );
		bpt::read_json( jsonStream, jsonReport );

		BOOST_CHECK_EQUAL( xmlReport.size(), jsonReport.get_child( "header" ).size() );

		std::vector< std::string >  xmlIds;
		std::vector< std::string > jsonIds;

		fillVectorXml( xmlReport, xmlIds, "<xmlattr>.id" );
		fillVectorJson( jsonReport.get_child( "header" ), jsonIds, "id" );

		BOOST_CHECK_EQUAL( xmlIds.size(), jsonIds.size() );
		for( size_t i = 0; i < xmlIds.size(); ++i )
			BOOST_CHECK_EQUAL( xmlIds.at(i), jsonIds.at(i) );
	}
}

BOOST_AUTO_TEST_SUITE_END()
