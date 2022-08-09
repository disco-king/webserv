#ifndef CONFIG_UTILS_HPP
# define CONFIG_UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <stack>
# include <fcntl.h>
# include <unistd.h>
# include <limits>
# include <map>
# include <set>

# define configmap std::map<std::string, void (ServerConfig::*)(std::vector<std::string>)>

typedef struct s_errorpages{
	std::vector<int>	codes;
	std::string			uri;
}	t_errorpages;

bool onlyDigits(std::string const &str);
unsigned int strtoip(std::string str);

#endif