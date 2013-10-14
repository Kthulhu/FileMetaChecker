#ifndef _FILEREADER_FILEREADER_HPP_
#define _FILEREADER_FILEREADER_HPP_

#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

namespace file_reader
{

class FileReader
{
public:
	FileReader( std::streambuf* inStream );

	size_t getLength();
	size_t getPosition();

	bool readData( char* data, const size_t size );

	void goToBegin();
	void goBack   ( const std::size_t size );
	void goForward( const std::size_t size );

	bool isEndOfFile( );

private:
	std::istream _fileBuffer;
};

}

#endif
