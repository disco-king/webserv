#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fcntl.h>
#include <unistd.h>

class ServerConfig;

class Config{
		std::string _path;
		int _fd;
		int _workers;

		std::string _fcontent;
		std::vector<std::string> _tokens;
		std::vector<ServerConfig> _servers;
	public:
		Config(std::string const &);
		~Config();

		std::vector<ServerConfig> &get_servers() const;
		std::string &get_path() const;
		std::string &get_fcontent();
		
		void parse_tokens();
		void main_parsing();
		void memory_clear();
};