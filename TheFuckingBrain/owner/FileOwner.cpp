#include "FileOwner.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

FileOwner::FileOwner() {

}
FileOwner::FileOwner(
	const char *filename, std::ios_base::openmode mode):
file(filename, mode)
	 {
	if (!file.is_open()) {
		throw std::runtime_error("Can't open file "+std::string(filename));
	}
}

FileOwner::~FileOwner() {
	file.close();
}

std::fstream * FileOwner::operator->()
{
	return &file;
}
