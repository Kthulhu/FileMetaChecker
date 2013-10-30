#include "Number.hpp"
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace basic_element
{

// Number::Number( const std::shared_ptr< spec_reader::SpecNode > node, const std::shared_ptr< Element > previous )
Number::Number( const std::shared_ptr< spec_reader::SpecNode > node, 
                const std::shared_ptr< Element > previous,
                const std::shared_ptr< Element > parent )
 : Element( node, previous, parent )
{
}

}
