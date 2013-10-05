
BOOST_AUTO_TEST_SUITE( report_test_transform_and_export )

BOOST_AUTO_TEST_CASE( report_transform_and_export )
{
	LOG_INFO( ">>> report_transform_and_export <<<" );
	Report elementReport;

	std::shared_ptr< ben::Number > numPtr1( new ben::Number( "intElem",   eSubTypeInt32 ) );
	std::shared_ptr< ben::Number > numPtr2( new ben::Number( "shortElem", eSubTypeInt16 ) );
	std::shared_ptr< bed::Data   > dataPtr( new bed::Data( "dataElem" ) );

	char buff1[] = { 0x00, 0x00, 0x00, 0x00 };
	char buff2[] = { 0x00, 0x00 };
	char buff3[] = { 0x00 };

	numPtr1->set( buff1, 4 );
	numPtr2->set( buff2, 2 );
	dataPtr->set( buff3, 1 );

	ReportNode node1 = elementReport.addRootElement( numPtr1 );
	node1 = node1.appendNext( dataPtr );

	ReportNode child1 = node1.appendChild( numPtr2 );
	child1 = child1.appendNext( numPtr1 );
	child1.appendChild( dataPtr );

	elementReport.addRootElement( numPtr2 );
	
	{
		Transform tr;
		tr.setBasicElementReport( elementReport );
	}
	{
		LOG_INFO( ">>> report_transform_transform <<<" );
		Transform tr( elementReport );
		tr.transformTree( Transform::eReportTypeXml );
	}
	{
		LOG_INFO( ">>> report_transform_and_export <<<" );
		Transform tr( elementReport );
		Export exporter( tr.transformTree( Transform::eReportTypeXml ) );
		//exporter.writeXmlFile( "report.xml" );
		//exporter.writeXmlFile( "report.xml", true );
	}
	{
		LOG_INFO( ">>> report_transform_and_export <<<" );
		Transform tr( elementReport );
		Export exporter( tr.transformTree( Transform::eReportTypeXml ) );
		LOG_INFO( exporter.get< Export::eExportTypeXml >() );
	}
}


BOOST_AUTO_TEST_CASE( report_export_xml_json )
{
	LOG_INFO( ">>> report_export_xml_json <<<" );
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

	bpt::ptree tree;
	std::istringstream isstream( jsonString );
	bpt::read_json( isstream, tree );
	
	Export exporter( tree );
	LOG_INFO( exporter.get< Export::eExportTypeXml >() );
	LOG_INFO( exporter.get< Export::eExportTypeXml >( true ) );
	LOG_INFO( exporter.get< Export::eExportTypeJson >() );
	LOG_INFO( exporter.get< Export::eExportTypeJson >( true ) );
	//exporter.write< Export::eExportTypeJson >( "report.json" );
	//exporter.write< Export::eExportTypeJson >( "report.json", true );
}


BOOST_AUTO_TEST_SUITE_END()
