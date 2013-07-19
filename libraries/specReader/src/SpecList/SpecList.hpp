#ifndef _SPECREADER_SPECLIST_HPP_
#define _SPECREADER_SPECLIST_HPP_

#include <Specification/Specification.hpp>

class SpecList
{
public:
	SpecList();
	~SpecList();

	void initDirectoryPaths();
	void addDirectoryPath( const std::string& directoryPath );
	void addSpecFromDirectories();
	void addSpecification( const Specification& spec );

	void clearSpecifications();
	void clearDirectories();

	Specification getSpec( const std::string& specId ) const;
	void getSpecList( std::map< std::string, std::string >& specIds ) const;
	
	size_t getSpecNumber() const;

private:
	std::vector< Specification >  _specifications;
	std::vector< std::string >    _directories;
};





#endif
