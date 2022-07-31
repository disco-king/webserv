#include "file_utils.hpp"

bool is_filename(std::string const &fname)
{
	struct stat data;
	stat(fname.c_str(), &data);
	return (S_ISREG(data.st_mode));
}