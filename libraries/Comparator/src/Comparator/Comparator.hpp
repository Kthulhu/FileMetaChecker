#ifndef _COMPARATOR_COMPARATOR_HPP_
#define _COMPARATOR_COMPARATOR_HPP_

#include <memory>

namespace basic_element
{
	class Element;
}

namespace spec_reader
{
	class Specification;
	class SpecNode;
}

namespace file_reader
{
	class FileReader;
}

namespace report_generator
{
	class Report;
}

namespace comparator
{

class Comparator
{
	typedef std::shared_ptr< basic_element::Element > PtrElement;
	typedef std::shared_ptr< spec_reader::SpecNode  > PtrSpecNode;
public:
	Comparator();
	void check( spec_reader::Specification& spec, file_reader::FileReader& reader, report_generator::Report& report );

private:
	bool       isInUnorderedGroup( const PtrElement element );
	PtrElement getNextParent     ( const PtrElement element, const PtrSpecNode node );
	
};

}

#endif
