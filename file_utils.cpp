#include "file_utils.hpp"

// bool is_filename(std::string const &fname)
// {
// 	struct stat data;
// 	stat(fname.c_str(), &data);
// 	return (S_ISREG(data.st_mode));
// }

bool is_filename(const std::string &file_name)
{
	struct stat file_stats;
	if (!stat(file_name.c_str(), &file_stats))
	{
		if (file_stats.st_mode & S_IFREG)
			return true;
	}
	return false;
}

bool is_dirname(const std::string &dir)
{
	struct stat stats;
	if (!stat(dir.c_str(), &stats))
	{
		if (stats.st_mode & S_IFDIR)
			return true;
	}
	return false;
}