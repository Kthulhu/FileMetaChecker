#include <Common/Element.hpp>
#include <Common/log.hpp>
#include <SpecReader/SpecNode.hpp>

#include <iostream>

namespace sr = spec_reader;

namespace basic_element
{

Element::Element( const std::shared_ptr< sr::SpecNode > node,
	              const std::shared_ptr< Element > previous,
	              const std::shared_ptr< Element > parent )
	: _parent        ( parent )
	, _previous      ( previous )
	, _specNode      ( node )
	, _id            ( node->getId() )
	, _label         ( node->getLabel() )
	, _uId           ( node->getUId() )
	, _size          ( 0 )
	, _iteration     ( 1 )
	, _countExpr     ( node->getCount() )
	, _requiredExpr  ( node->getRequirement() )
	, _groupSizeExpr ( node->getGroupSize() )
	, _values        ( node->getValues() )
	, _map           ( node->getMap() )
	, _rangeExpr     ( node->getRange() )
	, _repetExpr     ( node->getRepetitions() )
	, _type          ( node->getType() )
	, _displayType   ( node->getDisplayType() )
	, _status        ( eStatusNotChecked )
	, _error         ( "" )
	, _warning       ( "" )
	, _isGroup       ( node->isGroup() )
	, _isOrdered     ( node->isOrdered() )
	, _isOptional    ( node->isOptional() )
	, _isBigEndian   ( node->isBigEndian() )
	, _checkedGroup  ( false )
	, _data          ( nullptr )
{
	if( node->isRepeated() > 1 && ( previous.use_count() != 0 ) )
	{
		std::shared_ptr< Element > prev = previous;
		while( prev.use_count() != 0 )
		{
			if( prev->_id == node->getId() )
			{
				_iteration = prev->_iteration + 1;
				break;
			}
			else
			{
				if( prev->getParent().use_count() == 0 )
					break;
				prev = prev->getParent();
			}
		}
	}

	if( ! _previous.expired() )
		LOG_FATAL( _id << ": " << previous->_id );

	if( ! _parent.expired() )
		_parent.lock()->_children.push_back( std::make_shared< Element >( *this ) );

	LOG_ERROR( _id << ": " << &*this << " - Parent: " << &*_parent.lock() << " - Previous: " << &( *( _previous.lock().get() ) ) );
}

std::shared_ptr< spec_reader::SpecNode > Element::next( )
{
	// if element has been checked
	if( _status == eStatusNotChecked )
	{
		return _specNode.lock();
	}

	std::shared_ptr< Element > parent;
	// if parent exists (?)
	if( _parent.use_count() != 0 )
	{
		// copy the parent
		parent = _parent.lock();
	}
	
	// Optional element :
	// if element optional & invalid
	if( _isOptional && _status == eStatusInvalid )
	{
		// go to next SpecNode
		return _specNode.lock()->next();
	}
	
	// Unordered groups : if element status = Valid and parent is not ordered
	if( _status == eStatusValid && _parent.use_count() != 0 && ( ! parent->_specNode.lock()->isOrdered() ) )
	{
		// go to the first SpecNode of the childhood
		return parent->_specNode.lock()->firstChild();
	}
	
	// Groups :
	// if element has a group not already checked
	if( _specNode.lock()->isGroup() && ! _checkedGroup )
	{
		LOG_WARNING( "Element::next " << _id << ": IsGroup" );
		// it becomes checked
		_checkedGroup = true;
		// creates a pointer to this current element
		std::weak_ptr< spec_reader::SpecNode > child( _specNode.lock()->firstChild() );
		// go to the first child SpecNode
		return child.lock();
	}
	
	// Repetitions
	// get repetitions
	size_t count = _specNode.lock()->isRepeated();
	
	// if repeated element
	if( count > 1 )
	{
		LOG_WARNING( "Element::next " << _id << ": Repeated: " << _iteration );
		// and not repeated enough yet
		if( _iteration < count )
		{
			// go to the same SpecNode
			return _specNode.lock();
		}
	}
	
	// creates a pointer to the next SpecNode
	std::shared_ptr< sr::SpecNode > nextNode = _specNode.lock()->next();
	
	// in Unoredered groups : check if every nodes have been checked
	// if their is no more SpecNode after and parent exists
	if( nextNode == nullptr && _parent.use_count() != 0 )
	{
		LOG_WARNING( "Element::next " << _id << ": Last Element" );
		
		// if the current group (parent's children) is not ordered
		if( ! _parent.lock()->_specNode.lock()->isOrdered() )
		{
			LOG_WARNING( "Element::next " << _id << ": Unordered group check" );
			
			// create a list with the children IDs
			std::set< std::string > childIds = parent->_specNode.lock()->getChildrenNodes();

			// if the previous element exists (the current element is not the first)
			if( _previous.lock().get() != nullptr )
			{
				// creates a pointer to the previous element
				std::shared_ptr< Element > prev = _previous.lock();
				// while the previous element is not the parent
				while( prev->_id != parent->_id )
				{
					// for each ID in the children ID list
					for( auto id : childIds )				
					{
						// if the previous element's ID is equal to one ID of the list
						if( prev->_id == id && prev->_status == eStatusValid )
						{
							LOG_WARNING( "Element::next " << "childIds: " << id );
							// erase this ID from the list
							childIds.erase( id );
						}
					}
					// go to the previous element of the previous, etc..
					prev = prev->_previous.lock();
				}
			}			
			LOG_WARNING( "Element::next " << _id << ": End of check Unordered, remaining children: " << childIds.size() );
			// if it remains some IDs in the list
			if( childIds.size() != 0 )
			{
				LOG_ERROR( "Element::next " << _parent.lock()->_id );
				LOG_ERROR( "Element::next " << &*_parent.lock() );
				// every nodes haven't been checked : the parent is not valid
				_parent.lock()->_status = eStatusInvalidForUnordered;
			}
		}
		// go to the node after the parent
		return parent->next( );
	}
	LOG_WARNING( "Element::next " << _id << ": Next !" );
	// go to the next node
	return nextNode;
}

std::string Element::getStringStatus() const
{
	switch( _status )
	{
		case eStatusNotChecked         : return "not checked";
		case eStatusSkip               : return "skip";
		case eStatusPassOver           : return "pass over";
		case eStatusValid              : return "valid";
		case eStatusInvalid            : return "invalid";
		case eStatusInvalidButSkip     : return "invalid but skip";
		case eStatusInvalidButOptional : return "invalid but optional";
		case eStatusInvalidForUnordered: return "invalid for unordered";
	}
	return "";
}

void Element::set( const char* data, const size_t& size )
{
	_data = new char [size];
	_size = size;
	
	std::memcpy( _data, data, _size );
}

}


