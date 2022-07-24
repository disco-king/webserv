#pragma once

#include "Config.hpp"

#define configmap std::map<std::string, void (ServerConfig::*)(std::vector<std::string>)>

typedef struct s_listener{
	unsigned int host;
	int			 port;
}	t_listener;

typedef struct s_errorpages{
	std::vector<int>	codes;
	std::string			uri;
}	t_errorpages;