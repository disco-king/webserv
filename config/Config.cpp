#include "Config.hpp"

Config::Config(std::string const &path): _path(path){
	_fd = open(_path.c_str(), O_RDONLY);
	if (_fd < 0)
		std::cout << "error\n";
}

Config::~Config(){
	if (_fd > 0){
		close(_fd);
		_fd = 0;
	}
	_tokens.clear();
	_fcontent.clear();
}

void Config::parse_tokens(){
	std::string str;
	std::string tmp;
	std::string::size_type start;
	std::string::size_type end;
	std::stack<bool> brackets;

	
}