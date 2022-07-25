#pragma once

#include "ServerConfig.hpp"

class Config{
		std::string _path;
		std::string _fcontent;
		std::vector<std::string> _file;
		std::vector<ServerConfig> _servers;
	public:
		Config();
		Config(std::string const &);
		~Config();

		void setPath(std::string const &);
		void setFile();

		void parse();
		std::vector<std::string> filereader(const char *fname);
		std::vector<std::string> splitline(std::string);
		void print_file() const;

		class ConfigException: public std::exception{
				std::string	_msg;
			public:
				ConfigException(std::string const &);
				virtual const char *what() const throw();
		};
};
