#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ConfigUtils.hpp"
# include "ServerConfig.hpp"
# include "../request/Request.hpp"
# include "../utils/utils.hpp"

# define BUFF_SIZE 1024
# define DEFAULT_CONFIG_PATH "config/default.conf"

class ServerConfig;
class RequestConfig;

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

		void parse();
	
		std::vector<t_listen> getListeners() const;
		RequestConfig			getConfigForRequest(t_listen const, Request &) const;
		void					getServerForRequest(ServerConfig &, t_listen const,
									std::string const) const;
		std::vector<ServerConfig> getServers();
	
		std::vector<std::string> filereader(const char *fname);
		std::vector<std::string> splitline(std::string);

		void print_file() const;
		void print_config() const;

		class ConfigException: public std::exception{
				std::string	_msg;
			public:
				ConfigException(std::string const &);
				virtual const char *what() const throw();
		};
};

#endif