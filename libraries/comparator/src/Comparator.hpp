#ifndef _COMPARATOR_COMPARATOR_HPP_
#define _COMPARATOR_COMPARATOR_HPP_

#include <SpecList/SpecList.hpp>
#include <Report/Report.hpp>

namespace filereader
{
	class FileReader;
}


namespace comparator
{

class Comparator
{
public:
	Comparator( filereader::FileReader* file, const spec_reader::SpecList& specs );
	~Comparator();

	void compare( const std::string& specId, report_generator::Report& report );

protected:
	template < typename SubType >
	SubType getNodeSubType( const std::string& nodeType );

	void checkNode           ( const spec_reader::SpecNode& node, const bool& isFirstChild = false );
	void checkElementFromNode( const spec_reader::SpecNode& node, const bool& isFirstChild );
	void addElementToReport  ( const spec_reader::SpecNode& node, const std::shared_ptr< be::Element >& element, const bool& isFirstChild );
	
private:
	filereader::FileReader*       _file;
	spec_reader::SpecList         _specs;
	report_generator::Report*     _report;
	report_generator::ReportNode  _currentReportNode;
};


template < typename SubType >
SubType Comparator::getNodeSubType( const std::string& nodeType )
{
	if( nodeType == kAscii  ) return eDataTypeAscii;
	if( nodeType == kHexa   ) return eDataTypeHexa;
	if( nodeType == kRaw    ) return eDataTypeRaw;
	throw std::runtime_error( "getNodeSubType: Unknown type");
	return eDataTypeUnknown;
}


}

#endif
