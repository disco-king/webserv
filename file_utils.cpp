#include "file_utils.hpp"

bool is_filename(std::string const &fname)
{
	struct stat data;
	stat(fname.c_str(), &data);
	return (data.st_mode == S_IFREG);
}