#ifndef __file_owner
#define __file_owner
#include <fstream>
class FileOwner {
private:
	std::fstream file;
public:
	FileOwner();
	FileOwner(const char *filename,
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
	~FileOwner();
	std::fstream * operator->();
};
#endif