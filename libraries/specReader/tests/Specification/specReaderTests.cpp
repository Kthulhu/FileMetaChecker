#define BOOST_TEST_MODULE qc_spec_reader

#include <boost/test/unit_test.hpp>

#include <SpecList/SpecList.hpp>

using namespace boost::unit_test;
using namespace spec_reader;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

std::string id    = "id";
std::string label = "label";

BOOST_AUTO_TEST_SUITE( spec_reader_test_suite01 )

BOOST_AUTO_TEST_CASE( spec_reader )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_SUITE_END()

#include "specificationTests.hpp"
#include "specListTests.hpp"
#include "specNodeTests.hpp"
