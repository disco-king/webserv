#include "ServerConfig.hpp"

ServerConfig::ServerConfig(): _root(""), _client_buff_size(0),
	_autoindex(false), _alias_set(false) {}

ServerConfig::ServerConfig(ServerConfig const &ref){
	if (this != &ref){
		this->_listen = ref._listen;
		this->_root = ref._root;
		this->_server_name = ref._server_name;
		this->_error_page = ref._error_page;
		this->_client_buff_size = ref._client_buff_size;
		this->_cgi_param = ref._cgi_param;
		this->_cgi_pass = ref._cgi_pass;
		this->_location = ref._location;
		this->_methods = ref._methods;
		this->_autoindex = ref._autoindex;
		this->_index = ref._index;
		this->_alias = ref._alias;
		this->_alias_set = ref._alias_set;
	}
}

ServerConfig::~ServerConfig(){}

ServerConfig &ServerConfig::operator=(ServerConfig const &ref){
		if (this != &ref){
		this->_listen = ref._listen;
		this->_root = ref._root;
		this->_server_name = ref._server_name;
		this->_error_page = ref._error_page;
		this->_client_buff_size = ref._client_buff_size;
		this->_cgi_param = ref._cgi_param;
		this->_cgi_pass = ref._cgi_pass;
		this->_location = ref._location;
		this->_methods = ref._methods;
		this->_autoindex = ref._autoindex;
		this->_index = ref._index;
		this->_alias = ref._alias;
		this->_alias_set = ref._alias_set;
	}
}

void	ServerConfig::parse_server(std::vector<std::string> &file){
	
}