#include "string_utils.hpp"
#include <cctype>
#include <set>

std::set<std::string> text;
std::set<std::string> image;
std::set<std::string> audio;
std::set<std::string> video;
std::set<std::string> application;


void fill()
{
	text.insert("cmd");
	text.insert("css");
	text.insert("csv");
	text.insert("html");
	text.insert("php");

	image.insert("gif");
	image.insert("jpg");
	image.insert("png");

	video.insert("mp4");
	video.insert("mpeg");

	application.insert("pdf");
	application.insert("json");
	application.insert("zip");

	audio.insert("aac");
	audio.insert("mp3");
	audio.insert("aac");
}

std::string getFileType(std::string path)
{
	static bool time_to_fill = true;
	if(time_to_fill){
		time_to_fill = false;
		fill();
	}

	std::string res = "text/plain";

	size_t point = path.find_last_of('.');
	if(point == std::string::npos)
		return res;
	path = path.substr(point + 1);

	if(text.count(path))
		res = "text/" + path;
	else if(video.count(path))
		res = "video/" + path;
	else if(image.count(path))
		res = "image/" + path;
	else if(application.count(path))
		res = "application/" + path;
	else if(audio.count(path)){
		if(path == "mp3")
			res = "audio/mpeg";
		else if(path == "flac")
			res = "audio/ogg";
		else
			res = "audio/" + path;
	}
	return res;
}


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

std::string &to_lower(std::string &str)
{
	std::string::iterator end = str.end();
	for (std::string::iterator it = str.begin(); it != end; ++it)
		*it = std::tolower(*it);
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
	std::string::iterator ni, nend = needle.end();
	to_lower(needle);

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

void removeExtraSlashes(std::string &str, size_t index, size_t shift)
{
	int count = -1;
	for(; index < str.size(); ++index){
		if(str[index] == '/')
			++count;
		else if(count >= 0)
			break;
	}
	shift += count;
	count = --index;
	for(; index < str.size(); ++index){
		if(str[index] == '/' && (int)index != count)
			return(removeExtraSlashes(str, index, shift));
		str[index - shift] = str[index];
	}
	str.resize(str.size() - shift);
}