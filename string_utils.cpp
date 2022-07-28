#include "string_utils.hpp"
#include <cctype>

std::vector<std::string> split(std::string const &str, char del)
{
	size_t begin, end;
	std::vector<std::string> ret;

	begin = str.find_first_not_of(del);
	end = str.find_first_of(del, begin);
	while(begin != std::string::npos){
		ret.push_back(str.substr(begin, end - begin));
		begin = str.find_first_not_of(del, end);
		end = str.find_first_of(del, begin);
	}

	return ret;
}

std::string &strip(std::string &str)
{
	size_t begin, end;
	begin = str.find_first_not_of(" \t\r\t\v\f");
	end = str.find_last_not_of(" \t\r\t\v\f");
	str = str.substr(begin, end - begin + 1);
	return str;
}

bool ends_with(std::string const &str, std::string ending)
{
	std::string::const_reverse_iterator main_it = str.rbegin();
	std::string::const_reverse_iterator end_it = ending.rbegin();
	std::string::const_reverse_iterator end_lim = ending.rend();
	while(end_it != end_lim){
		if(*main_it++ != *end_it++)
			return false;
	}
	return true;
}

size_t find_string(std::string const& haystack, std::string needle)
{
	std::string::iterator ni = needle.begin();
	std::string::iterator nend = needle.end();
	for(; ni < nend; ++ni)
		*ni = std::tolower(*ni);

	std::string::const_iterator hi = haystack.begin();
	std::string::const_iterator hend = haystack.end();
	std::string::const_iterator tmp;
	for(; hi < hend; ++hi){
		tmp = hi;
		for(ni = needle.begin(); ni < nend; ++ni, ++tmp)
			if(std::tolower(*tmp) != *ni)
				break;
		if (ni == nend)
			return hi - haystack.begin();
	}
	return std::string::npos;
}