#ifndef _REPORT_REPORT_NODE_HPP_
#define _REPORT_REPORT_NODE_HPP_

#include <boost/property_tree/ptree.hpp>

#include <Element.hpp>

namespace be  = basic_element;
namespace bpt = boost::property_tree;

namespace report_generator
{

class ReportNode
{
public:
	
	ReportNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal );
	~ReportNode();

	// void appendNext ( std::shared_ptr< be::Element > element );	// @todo
	// void appendChild( std::shared_ptr< be::Element > element );	// @todo

	ReportNode next();
	ReportNode firstChild();
	// ReportNode* parent();	// @todo

	bool hasGroup();

	size_t getIndex();
	size_t getIndexTotal();

private:
	// ReportNode* _parent;		// @todo
	bpt::ptree::const_iterator _node;
	size_t _index;
	size_t _indexTotal;
};

}

#endif