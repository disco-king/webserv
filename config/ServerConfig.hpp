#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include "Config.hpp"
# include "../utils/utils.hpp"

class RequestConfig;

class ServerConfig{
		std::vector<t_listen> 	_listen;
		std::string					_root;
		std::vector<std::string> 	_server_name;
		std::map<int, std::string>	_error_page;
		int							_client_buff_size;

		std::map<std::string, std::string>	_cgi_param;
		std::string							_cgi_pass;
		std::map<std::string, ServerConfig> _location;
		std::set<std::string>				_methods;
		std::vector<std::string>			_index;
		bool								_autoindex;
		std::string							_alias;
		bool								_alias_set;

		static ServerConfig	_default_config;
		static configmap	_servermap;
		static configmap	_locmap;
		static configmap	initServMap();
		static configmap	initLocMap();


		void   	addListen(std::vector<std::string>);
		void   	addRoot(std::vector<std::string>);
		void   	addServerName(std::vector<std::string>);
		void   	addErrorPage(std::vector<std::string>);
		void   	addClientBuffSize(std::vector<std::string>);
		void	addCgiParam(std::vector<std::string>);
		void   	addCgiPass(std::vector<std::string>);
		void	addMethods(std::vector<std::string>);
		void	addIndex(std::vector<std::string>);
		void	addAutoIndex(std::vector<std::string>);
		void	addAlias(std::vector<std::string>);
	
	public:
		ServerConfig();
		ServerConfig(ServerConfig const &);
		ServerConfig &operator=(ServerConfig const &);
		virtual ~ServerConfig();

		std::vector<t_listen>				getListen() const;
		std::string							getRoot() const;
		std::vector<std::string>   			getServerName() const;
		std::map<int, std::string>			getErrorPage() const;
		int									getClientBodyBufferSize() const;
		std::map<std::string, std::string>	getCgiParam() const;
		std::string							getCgiPass() const;
		std::map<std::string, ServerConfig> getLocation() const;
		std::set<std::string>				getAllowedMethods() const;
		std::vector<std::string>			getIndex() const;
		bool								getAutoIndex() const;
		std::string							getAlias() const;
		bool								getAliasSet() const;
		static ServerConfig					&getDefaultConfig();
		ServerConfig						getRequestLoc(std::string const &, std::string &);

		void		parse_server(unsigned int &, std::vector<std::string> &);
		void		parse_loc(unsigned int &, std::vector<std::string> &);
		void		pass_members(ServerConfig &) const;
		static ServerConfig DefaultServerConfig(std::vector<std::string> &);

		class ServerConfigException: public std::exception {
				std::string _msg;
			public:
				ServerConfigException(std::string const &);
				virtual const char *what() const throw();
		};
};

std::ostream &operator<<(std::ostream &out, ServerConfig const &server);

// typedef void (ServerConfig::*addfunc)(std::vector<std::string>);
// typedef std::map<std::string, addfunc> configmap;

#endif